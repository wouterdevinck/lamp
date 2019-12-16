package provision

import (
	"errors"
	"factory/pki"
	"factory/protocol"
	"fmt"

	"github.com/tarm/serial"
)

type Context struct {
	protocol   *protocol.Protocol
	pki        *pki.Pki
	ica        string
	configPath string
	serial     string
}

func Provision(port string, baud int, configPath string, pki *pki.Pki, ica string) error {
	c := &serial.Config{Name: port, Baud: baud}
	s, err := serial.OpenPort(c)
	if err != nil {
		return err
	}
	p := protocol.CreateProtocol(s)
	ctx := &Context{
		protocol:   p,
		configPath: configPath,
		pki:        pki,
		ica:        ica,
	}
	sequence := []func(*Context) error{
		testConnectivity,
		checkRevision,
		getSerial,
		writeConfigZone,
		writeDataZone,
		restart,
	}
	for _, step := range sequence {
		err = step(ctx)
		if err != nil {
			return err
		}
	}
	return nil
}

func testConnectivity(ctx *Context) error {
	t, err := ctx.protocol.TestCommunication()
	if err != nil {
		return err
	}
	if !t {
		return errors.New("Failed to connect to device")
	}
	return nil
}

func checkRevision(ctx *Context) error {
	rev, err := ctx.protocol.GetRevision()
	if err != nil {
		return err
	}
	fmt.Printf("Revision: %s\n", rev)
	if rev != "00006002" {
		return errors.New("Connected device is not an ATECC608A")
	}
	return nil
}

func getSerial(ctx *Context) error {
	ser, err := ctx.protocol.GetSerial()
	if err != nil {
		return err
	}
	fmt.Printf("Serial: %s\n", ser)
	ctx.serial = ser
	return nil
}

func restart(ctx *Context) error {
	err := ctx.protocol.Restart()
	if err != nil {
		return err
	}
	fmt.Println("Restarting device")
	return nil
}
