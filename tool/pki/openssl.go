package pki

import "factory/helper"

type CA int

const (
	Root         CA = 0
	Intermediate    = 1
)

func (p *Pki) createCsr(keyPath string, csrPath string, subj string) error {
	_, err := helper.RunCommand(
		"openssl", "req",
		"-sha256", "-new",
		"-key", keyPath,
		"-out", csrPath,
		"-subj", subj,
	)
	return err
}

func (p *Pki) signCertificate(csrPath string, cerPath string, ca CA) error {
	_, err := helper.RunCommand(
		"openssl", "ca", "-batch",
		"-config", ca.conf(),
		"-notext",
		"-in", csrPath,
		"-out", cerPath,
	)
	return err
}

func (ca CA) conf() string {
	conf := [...]string{
		"root.conf",
		"intermediate.conf"}
	return conf[ca]
}

func (p *Pki) createCertificate(keyPath string, cerPath string, subj string) error {
	_, err := helper.RunCommand(
		"openssl", "req",
		"-sha256", "-new",
		"-x509",
		"-days", "36500",
		"-key", keyPath,
		"-out", cerPath,
		"-subj", subj,
	)
	return err
}
