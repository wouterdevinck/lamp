package pki

import (
	"factory/helper"
)

const hsmConf string = "./hsm.conf"

func (p *Pki) createCsr(key *Key, csrPath string, subj string) error {
	env := make(map[string]string)
	params := []string{
		"req",
		"-sha256", "-new",
		"-key", key.keyRef,
		"-out", csrPath,
		"-subj", subj,
	}
	if key.useHsm {
		env["OPENSSL_CONF"] = hsmConf
		env["HSM_PIN"] = p.hsmPin
		params = append(params, "-engine", "pkcs11", "-keyform", "engine")
	}
	_, err := helper.RunCommand(env, "openssl", params...)
	return err
}

func (p *Pki) signCertificate(csrPath string, cerPath string, ca *CA) error {
	env := make(map[string]string)
	params := []string{
		"ca", "-batch",
		"-config", ca.config,
		"-notext",
		"-in", csrPath,
		"-out", cerPath,
	}
	env["CA_DIR"] = ca.dir
	env["CA_KEY"] = ca.key.keyRef
	if ca.key.useHsm {
		env["OPENSSL_CONF"] = hsmConf
		env["HSM_PIN"] = p.hsmPin
		params = append(params, "-engine", "pkcs11", "-keyform", "engine")
	}
	_, err := helper.RunCommand(env, "openssl", params...)
	return err
}

func (p *Pki) createCertificate(key *Key, cerPath string, subj string) error {
	env := make(map[string]string)
	params := []string{
		"req",
		"-sha256", "-new",
		"-x509",
		"-days", "36500",
		"-key", key.keyRef,
		"-out", cerPath,
		"-subj", subj,
	}
	if key.useHsm {
		env["OPENSSL_CONF"] = hsmConf
		env["HSM_PIN"] = p.hsmPin
		params = append(params, "-engine", "pkcs11", "-keyform", "engine")
	}
	_, err := helper.RunCommand(env, "openssl", params...)
	return err
}
