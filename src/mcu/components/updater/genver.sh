#!/bin/bash
set -e

if [ $# -ne 2 ]; then 
   echo "Wrong number of parameters"
   exit 1
fi

printf "#pragma once\n#define FPGA_HASH \"%s\"\n#define LAMP_VERSION \"%s\"\n" \
	  `cat $1` `git describe --tags --dirty` > $2