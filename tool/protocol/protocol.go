package protocol

import (
	"errors"
	"factory/model"
	"strings"

	"github.com/tarm/serial"
)

type Protocol struct {
	serial *serial.Port
	resps  chan string
}

func CreateProtocol(serial *serial.Port) *Protocol {
	p := &Protocol{
		serial: serial,
		resps:  make(chan string)}
	go p.listen()
	return p
}

func (p *Protocol) TestCommunication() (bool, error) {
	_, e := p.runCommand("test")
	if e != nil {
		return false, e
	}
	return true, nil
}

func (p *Protocol) GetRevision() (string, error) {
	return p.getString("revision")
}

// func (p *Protocol) GetRandom() (string, error) {
// 	return p.getString("random")
// }

func (p *Protocol) GetSerial() (string, error) {
	return p.getString("serial")
}

func (p *Protocol) ReadConfig() (string, error) {
	return p.getString("readconfig")
}

func (p *Protocol) WriteConfig(config string) error {
	_, e := p.runCommand("writeconfig", config)
	return e
}

func (p *Protocol) IsConfigLocked() (bool, error) {
	r, e := p.runCommand("configlocked")
	if e != nil {
		return false, e
	}
	return r[0] == "true", nil
}

func (p *Protocol) LockConfig() error {
	_, e := p.runCommand("lockconfig")
	return e
}

func (p *Protocol) GenKey() (string, error) {
	return p.getString("genkey")
}

func (p *Protocol) WriteIoProtectionKey() error {
	_, e := p.runCommand("iokey")
	return e
}

func (p *Protocol) SignCsr(template *model.CsrTemplate) (string, error) {
	return p.getString("signcsr", template.ToString())
}

func (p *Protocol) WriteCert(cert string) error {
	_, e := p.runCommand("writecert", cert)
	return e
}

func (p *Protocol) IsDataLocked() (bool, error) {
	r, e := p.runCommand("datalocked")
	if e != nil {
		return false, e
	}
	return r[0] == "true", nil
}

func (p *Protocol) LockData() error {
	_, e := p.runCommand("lockdata")
	return e
}

func (p *Protocol) Restart() error {
	_, e := p.runCommand("restart")
	return e
}

func (p *Protocol) listen() error {
	data := make([]byte, 512)
	line := ""
	for {
		n, err := p.serial.Read(data)
		if err != nil {
			return err
		}
		buffer := string(data[:n])
		// log.Println(buffer)
		pos := strings.Index(buffer, "\n")
		for pos > -1 {
			line += buffer[0:pos]
			p.resps <- line
			line = ""
			buffer = buffer[pos+1:]
			pos = strings.Index(buffer, "\n")
		}
		line += buffer
	}
}

func (p *Protocol) send(message string) error {
	// log.Println(message)
	_, err := p.serial.Write([]byte(message + "\n"))
	return err
}

func (p *Protocol) runCommand(command string, parameters ...string) ([]string, error) {
	message := command
	for _, param := range parameters {
		message += " " + param
	}
	e := p.send(message)
	if e != nil {
		return nil, e
	}
	r := <-p.resps
	a := strings.Split(r, " ")
	if a[0] != command || a[1] != "ok" {
		return nil, errors.New("Unexpected response: " + r)
	}
	return a[2:], nil
}

func (p *Protocol) getString(command string, parameters ...string) (string, error) {
	r, e := p.runCommand(command, parameters...)
	if e != nil {
		return "", e
	}
	return r[0], nil
}
