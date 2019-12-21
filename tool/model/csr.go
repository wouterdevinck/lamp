package model

import (
	"bytes"
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/rand"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/hex"
	"encoding/pem"
	"errors"
	"fmt"
)

type CsrTemplate struct {
	template       string
	tbsIndex       int
	tbsCount       int
	publicKeyIndex int
	publicKeyCount int
	signatureIndex int
	signatureCount int
}

func CreateCsrTemplate(O string, CN string) (*CsrTemplate, error) {
	key, err := ecdsa.GenerateKey(elliptic.P256(), rand.Reader)
	if err != nil {
		return nil, err
	}
	info := x509.CertificateRequest{
		Subject: pkix.Name{
			CommonName:   CN,
			Organization: []string{O},
		},
		SignatureAlgorithm: x509.ECDSAWithSHA256}
	template, err := x509.CreateCertificateRequest(rand.Reader, &info, key)
	if err != nil {
		return nil, err
	}
	tmp := new(CsrTemplate)
	csr, err := x509.ParseCertificateRequest(template)
	if err != nil {
		return nil, err
	}
	tmp.template = hex.EncodeToString(template)
	pubkey := &key.PublicKey
	publicKey := elliptic.Marshal(pubkey, pubkey.X, pubkey.Y)
	tmp.tbsIndex = bytes.Index(template, csr.RawTBSCertificateRequest)
	tmp.publicKeyIndex = bytes.Index(template, publicKey)
	tmp.signatureIndex = bytes.Index(template, csr.Signature)
	if tmp.tbsIndex == -1 || tmp.publicKeyIndex == -1 || tmp.signatureIndex == -1 {
		return nil, errors.New("Missing information in CSR template")
	}
	tmp.publicKeyIndex += 1
	tmp.signatureIndex -= 3
	tmp.tbsCount = len(csr.RawTBSCertificateRequest)
	tmp.publicKeyCount = len(publicKey) - 1
	tmp.signatureCount = len(csr.Signature) + 3
	return tmp, nil
}

func (tmp *CsrTemplate) ToString() string {
	return fmt.Sprintf("%s %d %d %d %d %d %d",
		tmp.template,
		tmp.tbsIndex, tmp.tbsCount,
		tmp.publicKeyIndex, tmp.publicKeyCount,
		tmp.signatureIndex, tmp.signatureCount)
}

func GetCnFromCsr(csrPem string) (string, error) {
	block, _ := pem.Decode([]byte(csrPem))
	if block == nil || block.Type != "CERTIFICATE REQUEST" {
		return "", errors.New("Failed to parse CSR")
	}
	csr, err := x509.ParseCertificateRequest(block.Bytes)
	if err != nil {
		return "", err
	}
	return csr.Subject.CommonName, nil
}
