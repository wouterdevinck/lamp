package main

import (
	"factory/pki"
	"factory/provision"
	"flag"
	"fmt"
	"log"
	"os"
)

var version = "unknown"

func main() {
	port := flag.String("port", "/dev/ttyUSB1", "serial port to connect to")
	baud := flag.Int("baud", 115200, "baudrate of serial port")
	config := flag.String("config", "config.json", "path to configuration file")
	pkipath := flag.String("pkipath", "/var/lib/pki", "path to PKI database")
	hsmpin := flag.String("hsmpin", "648219", "user PIN for Nitrokey HSM")
	icaname := flag.String("icaname", "lamp", "name of the intermediate CA")

	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, `Lamp factory tool

Usage: %s [options] <command>

Commands:

  version          display version number
  pki setup        create the root and intermediate CAs
  pki verify <cn>  create a verification certificate
  provision        provision device certificate

Options:

`, os.Args[0])
		flag.PrintDefaults()
	}

	flag.Parse()
	command := flag.Arg(0)

	pki := pki.CreatePki(*pkipath, *hsmpin)

	var err error

	switch command {
	case "version":
		fmt.Printf("Version: %s\n", version)
	case "pki":
		subcommand := flag.Arg(1)
		switch subcommand {
		case "setup":
			err = pki.Setup()
		case "verify":
			cn := flag.Arg(2)
			cer, err := pki.CreateCertificate(cn, *icaname)
			if err != nil {
				log.Fatal(err)
			}
			fmt.Printf("Certificate: \n%s\n", cer)
		default:
			flag.Usage()
			os.Exit(1)
		}
	case "provision":
		err = provision.Provision(*port, *baud, *config, pki, *icaname)
	default:
		flag.Usage()
		os.Exit(1)
	}

	if err != nil {
		log.Fatal(err)
	}
}
