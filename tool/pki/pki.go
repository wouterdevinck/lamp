package pki

import (
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rand"
	"crypto/x509"
	"encoding/pem"
	"errors"
	"factory/helper"
	"factory/model"
	"os"
	"path"
)

const (
	caDir        string = "ca"
	icaDir              = "ica"
	cerDir              = "certs"
	organization        = "Wouter IOT"
	subj                = "/O=" + organization + "/CN="
)

type Pki struct {
	baseDir string
	hsmPin  string
	RCA     *CA
	ICAs    map[string]*CA
}

type CA struct {
	dir    string
	config string
	key    *Key
}

type Key struct {
	useHsm bool
	keyRef string
}

func CreatePki(baseDir string, hsmPin string) *Pki {
	// TODO Read the list of CAs from a file
	p := &Pki{
		baseDir: baseDir,
		hsmPin:  hsmPin,
		RCA: &CA{
			dir:    path.Join(baseDir, caDir),
			config: "root.conf",
			key: &Key{
				useHsm: true,
				keyRef: "0:1",
			},
		},
		ICAs: map[string]*CA{
			"lamp": {
				dir:    path.Join(baseDir, icaDir, "lamp"),
				config: "intermediate.conf",
				key: &Key{
					useHsm: true,
					keyRef: "0:2",
				},
			},
		},
	}
	return p
}

func (p *Pki) GetOrganization() string {
	return organization
}

func (p *Pki) Setup() error {
	err := os.MkdirAll(p.baseDir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return err
	}
	err = p.createRootCa()
	if err != nil {
		return err
	}
	for _, ica := range p.ICAs {
		err = p.createIntermediateCa(ica)
		if err != nil {
			return err
		}
	}
	err = os.Mkdir(path.Join(p.baseDir, cerDir), os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return err
	}
	return nil
}

func (p *Pki) SignCertificate(csr string, ica string) (string, error) {
	cn, err := model.GetCnFromCsr(csr)
	if err != nil {
		return "", err
	}
	dir := path.Join(p.baseDir, cerDir, cn)
	csrPath := path.Join(dir, "device.csr")
	cerPath := path.Join(dir, "device.cer")
	if _, err = os.Stat(cerPath); !os.IsNotExist(err) {
		return "", errors.New("certificate already exists")
	}
	err = os.Mkdir(dir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return "", err
	}
	err = helper.WriteFile(csrPath, csr)
	if err != nil {
		return "", err
	}
	err = p.signCertificate(csrPath, cerPath, p.ICAs[ica])
	if err != nil {
		return "", err
	}
	pem, err := helper.ReadFile(cerPath)
	if err != nil {
		return "", err
	}
	return pem, nil
}

func (p *Pki) CreateCertificate(cn string, ica string) (string, error) {
	dir := path.Join(p.baseDir, cerDir, cn)
	csrPath := path.Join(dir, "device.csr")
	cerPath := path.Join(dir, "device.cer")
	key := &Key{
		useHsm: false,
		keyRef: path.Join(dir, "device.key"),
	}
	if _, err := os.Stat(cerPath); !os.IsNotExist(err) {
		return "", errors.New("certificate already exists")
	}
	err := os.Mkdir(dir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return "", err
	}
	err = p.createKey(dir, key)
	if err != nil {
		return "", err
	}
	err = p.createCsr(key, csrPath, subj+cn)
	if err != nil {
		return "", err
	}
	err = p.signCertificate(csrPath, cerPath, p.ICAs[ica])
	if err != nil {
		return "", err
	}
	pem, err := helper.ReadFile(cerPath)
	if err != nil {
		return "", err
	}
	return pem, nil
}

func (p *Pki) GetCertificate(cn string) (string, error) {
	dir := path.Join(p.baseDir, cerDir, cn)
	cerPath := path.Join(dir, "device.cer")
	if _, err := os.Stat(cerPath); os.IsNotExist(err) {
		return "", errors.New("certificate not found")
	}
	pem, err := helper.ReadFile(cerPath)
	if err != nil {
		return "", err
	}
	return pem, nil
}

func (p *Pki) createRootCa() error {
	dir := p.RCA.dir
	cerPath := path.Join(dir, "root.cer")
	err := p.createDb(dir)
	if err != nil {
		return err
	}
	err = p.createKey(dir, p.RCA.key)
	if err != nil {
		return err
	}
	return p.createCertificate(p.RCA.key, cerPath, subj+"Root CA")
}

func (p *Pki) createIntermediateCa(ca *CA) error {
	dir := ca.dir
	csrPath := path.Join(dir, "intermediate.csr")
	cerPath := path.Join(dir, "intermediate.cer")
	err := p.createDb(dir)
	if err != nil {
		return err
	}
	err = p.createKey(dir, ca.key)
	if err != nil {
		return err
	}
	err = p.createCsr(ca.key, csrPath, subj+"Intermediate CA")
	if err != nil {
		return err
	}
	return p.signCertificate(csrPath, cerPath, p.RCA)
}

func (p *Pki) createDb(dir string) error {
	dbDir := path.Join(dir, "db")
	serialPath := path.Join(dbDir, "certserial")
	indexPath := path.Join(dbDir, "certindex")
	err := os.MkdirAll(dbDir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return err
	}
	err = helper.WriteFile(serialPath, "1000")
	if err != nil {
		return err
	}
	_, err = os.Create(indexPath)
	if err != nil {
		return err
	}
	return nil
}

func (p *Pki) createKey(dir string, key *Key) error {
	if !key.useHsm {
		pk, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
		if err != nil {
			return err
		}
		pem, err := p.pemEncodePrivateKey(pk)
		if err != nil {
			return err
		}
		err = helper.WriteFile(key.keyRef, pem)
		if err != nil {
			return err
		}
	}
	return nil
}

func (p *Pki) pemEncodePrivateKey(privateKey *ecdsa.PrivateKey) (string, error) {
	x509EncodedPub, err := x509.MarshalECPrivateKey(privateKey)
	if err != nil {
		return "", err
	}
	pemEncodedPub := pem.EncodeToMemory(&pem.Block{
		Type:  "EC PRIVATE KEY",
		Bytes: x509EncodedPub})
	return string(pemEncodedPub), nil
}
