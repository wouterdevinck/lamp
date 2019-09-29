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
	caDir  string = "ca"
	icaDir        = "ica"
	cerDir        = "certs"
	subj          = "/C=BE/ST=West-Vlaanderen/L=Lauwe/O=Wouter Devinck/OU=IOT/CN="
)

type Pki struct {
	baseDir string
}

func CreatePki(baseDir string) *Pki {
	p := &Pki{baseDir: baseDir}
	return p
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
	err = p.createIntermediateCa()
	if err != nil {
		return err
	}
	err = os.Mkdir(path.Join(p.baseDir, cerDir), os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return err
	}
	return nil
}

func (p *Pki) SignCertificate(csr string) (string, error) {
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
	err = p.signCertificate(csrPath, cerPath, Intermediate)
	if err != nil {
		return "", err
	}
	pem, err := helper.ReadFile(cerPath)
	if err != nil {
		return "", err
	}
	return pem, nil
}

func (p *Pki) CreateCertificate(cn string) (string, error) {
	dir := path.Join(p.baseDir, cerDir, cn)
	keyPath := path.Join(dir, "device.key")
	csrPath := path.Join(dir, "device.csr")
	cerPath := path.Join(dir, "device.cer")
	if _, err := os.Stat(cerPath); !os.IsNotExist(err) {
		return "", errors.New("certificate already exists")
	}
	err := os.Mkdir(dir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return "", err
	}
	err = p.createKey(keyPath)
	if err != nil {
		return "", err
	}
	err = p.createCsr(keyPath, csrPath, subj+cn)
	if err != nil {
		return "", err
	}
	err = p.signCertificate(csrPath, cerPath, Intermediate)
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
	dir := path.Join(p.baseDir, caDir)
	keyPath := path.Join(dir, "root.key")
	cerPath := path.Join(dir, "root.cer")
	err := p.createDb(dir)
	if err != nil {
		return err
	}
	err = p.createKey(keyPath)
	if err != nil {
		return err
	}
	return p.createCertificate(keyPath, cerPath, subj+"Root CA")
}

func (p *Pki) createIntermediateCa() error {
	dir := path.Join(p.baseDir, icaDir)
	keyPath := path.Join(dir, "intermediate.key")
	csrPath := path.Join(dir, "intermediate.csr")
	cerPath := path.Join(dir, "intermediate.cer")
	err := p.createDb(dir)
	if err != nil {
		return err
	}
	err = p.createKey(keyPath)
	if err != nil {
		return err
	}
	err = p.createCsr(keyPath, csrPath, subj+"Intermediate CA")
	if err != nil {
		return err
	}
	return p.signCertificate(csrPath, cerPath, Root)
}

func (p *Pki) createDb(dir string) error {
	dbDir := path.Join(dir, "db")
	serialPath := path.Join(dbDir, "certserial")
	indexPath := path.Join(dbDir, "certindex")
	err := os.Mkdir(dir, os.ModePerm)
	if err != nil && !os.IsExist(err) {
		return err
	}
	err = os.Mkdir(dbDir, os.ModePerm)
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

func (p *Pki) createKey(path string) error {
	key, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
	if err != nil {
		return err
	}
	pem, err := p.pemEncodePrivateKey(key)
	if err != nil {
		return err
	}
	return helper.WriteFile(path, pem)
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
