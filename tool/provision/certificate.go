package provision

import (
	"crypto/ecdsa"
	"crypto/elliptic"
	"crypto/x509"
	"encoding/hex"
	"encoding/pem"
	"factory/model"
	"fmt"
	"math/big"
)

func certificate(ctx *Context) error {

	// Check if the data zone is locked
	locked, err := ctx.protocol.IsDataLocked()
	if err != nil {
		return err
	}

	var cert string

	if !locked {

		// Generate a new keypair
		key, err := ctx.protocol.GenKey()
		if err != nil {
			return err
		}
		fmt.Println("\nKeypair generated in device\n")
		err = printPublicKey(key)
		if err != nil {
			return err
		}

		// Create a template for the CSR
		tmp, err := model.CreateCsrTemplate(organization, ctx.serial)
		if err != nil {
			return err
		}

		// Let the device sign the CSR
		r, err := ctx.protocol.SignCsr(tmp)
		if err != nil {
			return err
		}
		bytes, err := hex.DecodeString(r)
		if err != nil {
			return err
		}
		csr := string(pem.EncodeToMemory(&pem.Block{
			Type:  "CERTIFICATE REQUEST",
			Bytes: bytes,
		}))
		fmt.Println("\nCSR signed by device\n")
		fmt.Println(csr)

		// Send the CSR to the PKI
		cert, err = ctx.pki.SignCertificate(csr)
		if err != nil {
			return err
		}
		fmt.Println("\nCertificate signed by PKI\n")
		fmt.Println(cert)
		fmt.Println("")

	} else {

		// This flow is assuming the certificate is kept in
		// flash on the device, rather than on the ATECC608A
		// secure element. If the data zone is locked, but
		// the device has still entered factory mode, this
		// means the certificate in flash was lost and needs
		// to be restored. No new keys are or certificates are
		// generated, so this currently only works if the PKI
		// still has the certificate in its database.

		cert, err = ctx.pki.GetCertificate(ctx.serial)
		if err != nil {
			return err
		}
		fmt.Println("\nExisting certificate retrieved from PKI\n")
		fmt.Println(cert)
		fmt.Println("")

	}

	// Write the signed certificate to the device
	block, _ := pem.Decode([]byte(cert))
	certString := hex.EncodeToString(block.Bytes)
	err = ctx.protocol.WriteCert(certString)
	if err != nil {
		return err
	}
	fmt.Println("Signed certificate written to device")

	if !locked {

		// Lock the data zone
		err := ctx.protocol.LockData()
		if err != nil {
			return err
		}
		fmt.Println("Device data zone locked")

	}

	return nil

}

func printPublicKey(key string) error {
	data, err := hex.DecodeString(key)
	if err != nil {
		return err
	}
	pub := ecdsa.PublicKey{
		Curve: elliptic.P256(),
		X:     new(big.Int).SetBytes(data[0:32]),
		Y:     new(big.Int).SetBytes(data[32:64]),
	}
	pem, err := pemEncodePublicKey(&pub)
	if err != nil {
		return err
	}
	fmt.Println(pem)
	return nil
}

func pemEncodePublicKey(publicKey *ecdsa.PublicKey) (string, error) {
	bytes, err := x509.MarshalPKIXPublicKey(publicKey)
	if err != nil {
		return "", err
	}
	pemEncodedPub := pem.EncodeToMemory(&pem.Block{
		Type:  "EC PUBLIC KEY",
		Bytes: bytes,
	})
	return string(pemEncodedPub), nil
}
