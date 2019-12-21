#!/bin/bash
set -e

if [ $# -ne 2 ]; then 
   echo "wrong number of parameters"
   exit 1
fi

SRCDIR=$1
OUTDIR=$2

mkdir -p ${OUTDIR}

ROOT=`pwd`/

pushd ${SRCDIR}

go build -a -tags netgo -v -ldflags \
   "-X main.version=`git describe --tags --dirty`" \
   -o ${ROOT}${OUTDIR}factory ./main.go

popd