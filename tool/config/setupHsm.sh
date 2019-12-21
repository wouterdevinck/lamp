#!/bin/bash
set -e

# Secrets
SOPIN=""
PIN=""
DKEKPWD1=""
DKEKPWD2=""
DKEKPWD3=""

# Storage
DIR="/var/lib/pki/hsm"
mkdir -p $DIR

# Install tools
sudo apt-get install pcscd pcsc-tools opensc libengine-pkcs11-openssl -y

# Initialize the Nitrokey HSM
sc-hsm-tool --initialize --so-pin $SOPIN --pin $PIN --dkek-shares 3

# Create a backup file (can be printed and stored)
cat <<EOF > $DIR/backup.txt
SO PIN = $SOPIN
USER PIN = $PIN

EOF

# Create 3 shares of the DKEK
function createDkekShare() {
    sc-hsm-tool --create-dkek-share $DIR/dkek-share-$1.bin --password $2
    sc-hsm-tool --import-dkek-share $DIR/dkek-share-$1.bin --password $2
    cat <<EOF >> $DIR/backup.txt
DKEK SHARE
PWD = $2
$(openssl base64 -in $DIR/dkek-share-$1.bin)

EOF
    rm $DIR/dkek-share-$1.bin
}
createDkekShare 1 $DKEKPWD1
createDkekShare 2 $DKEKPWD2
createDkekShare 3 $DKEKPWD3

# Create keys
function createKey() {
    pkcs11-tool --login --pin $PIN --keypairgen --key-type EC:prime256v1 --id $1 --label "$2"
    sc-hsm-tool --wrap-key $DIR/backup-$1.bin --key-reference $1 --pin $PIN
    cat <<EOF >> $DIR/backup.txt
KEY: $2
$(openssl base64 -in $DIR/backup-$1.bin)

EOF
    rm $DIR/backup-$1.bin
}
createKey 1 "Wouter IoT Root CA"
createKey 2 "Wouter IoT Lamp Intermediate CA"


# Other useful commands
# ---------------------
# pcsc_scan
# opensc-tool --list-readers
# pkcs11-tool --list-slots
# pkcs15-tool --list-pins
# pkcs15-tool --change-pin -a 02
# pkcs11-tool --login --login-type so --so-pin 3537363231383830 --change-pin --new-pin 0123456789ABCDEF
# pkcs11-tool --list-objects
# pkcs11-tool --test --login --pin 648219
# pkcs15-tool --dump
# pkcs15-tool --read-public-key 1