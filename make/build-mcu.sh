#!/bin/bash
set -e

if [ $# -ne 3 ]; then 
   echo "Wrong number of parameters"
   printUsage
   exit 1
fi

SRCDIR=$2
OUTDIR=$3

mkdir -p ${OUTDIR}

function printUsage {
  echo "Usage: $0 reconfigure|build|menuconfig"
}

function runIdf {
   pushd ${SRCDIR}
   idf.py -B ${OUTDIR} $1
   popd
}

case $1 in
"reconfigure")
  runIdf reconfigure
  ;;
"build")
  runIdf build
  ;;
"menuconfig")
  runIdf menuconfig
  ;;
*)
  echo "Unknown command"
  printUsage
  ;;
esac