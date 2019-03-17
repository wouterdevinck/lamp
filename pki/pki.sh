#!/usr/bin/env bash
set -e

SUBJ="/C=BE/ST=West-Vlaanderen/L=Lauwe/O=Wouter Devinck/OU=IOT/CN="
PFXPWD="1234"

DIRCA="ca"
DIRICA="ica"
DIRDEV="certs"

function printUsage {
  echo "Usage: $0 cmd"
  echo "   clean    - Start from scratch."
  echo "   gen-ca   - Generate a new root CA."
  echo "   gen-ica  - Generate a new intermediate CA, signed by the root CA."
  echo "   gen <CN> - Generate a new certificate, signed by the intermediate CA."
  echo ""
}

function genKey {
  # openssl genrsa -out $1 4096
  openssl ecparam -genkey -noout -name prime256v1 -out $1
}

case $1 in

"clean")
  rm -rf $DIRCA $DIRICA $DIRDEV
  ;;

"gen-ca")
  mkdir -p $DIRCA/db
  echo 1000 > $DIRCA/db/certserial
  touch $DIRCA/db/certindex
  genKey $DIRCA/root.key
  openssl req -sha256 -new -x509 -days 36500 -key $DIRCA/root.key -out $DIRCA/root.cer -subj "${SUBJ}Root CA"
  ;;

"gen-ica")
  mkdir -p $DIRICA/db
  echo 1000 > $DIRICA/db/certserial
  touch $DIRICA/db/certindex
  genKey $DIRICA/intermediate.key
  openssl req -sha256 -new -key $DIRICA/intermediate.key -out $DIRICA/intermediate.csr -subj "${SUBJ}Intermediate CA"
  openssl ca -batch -config root.conf -notext -in $DIRICA/intermediate.csr -out $DIRICA/intermediate.cer
  ;;

"gen")
  if [[ ! $# -eq 2 ]] ; then
    printUsage
    exit 1
  fi
  mkdir -p $DIRDEV/$2
  genKey $DIRDEV/$2/device.key
  openssl req -new -sha256 -key $DIRDEV/$2/device.key -out $DIRDEV/$2/device.csr -subj "${SUBJ}$2"
  openssl ca -batch -config intermediate.conf -notext -in $DIRDEV/$2/device.csr -out $DIRDEV/$2/device.cer
  openssl pkcs12 -export -out $DIRDEV/$2/certificate.pfx -inkey $DIRDEV/$2/device.key -in $DIRDEV/$2/device.cer -password pass:$PFXPWD
  ;;

*)
  printUsage
  ;;

esac
