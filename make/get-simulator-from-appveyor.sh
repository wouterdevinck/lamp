#!/bin/bash
set -e

BASEURL=https://ci.appveyor.com/api
PROJECT=wouterdevinck/lamp
ARTIFACTSRC=test%2Fsimulator%2FLamp.Simulator%2Fbin%2Fsimulator.zip
ARTIFACTDIR=out/simulator
ARTIFACT=$ARTIFACTDIR/simulator.zip

if [ "$#" -ne 2 ]; then
    echo "Usage: ./get-simulator-from-appveyor.sh <token> <version>"
    exit 2
fi

TOKEN=$1
VERSION=$2

i="0"
STATUS="null"

while [[ $i -lt 300 && "$STATUS" = "null" ]]
do
  echo "[INFO] AppVeyor: checking build status"
  RESPONSE=`curl -s -H "Authorization: Bearer $TOKEN" \
   -H "Accept: application/json" \
   -X GET $BASEURL/projects/$PROJECT/build/$VERSION`
  STATUS=`echo $RESPONSE | jq -r .build.status`
  i=$[$i+1]
  sleep 1s
done

if [ "$STATUS" = "null" ]
then
  echo "[ERROR] AppVeyor: timeout!"
  exit 2
fi

if [ "$STATUS" = "success" ]
then
  JOBID=`echo $RESPONSE | jq -r .build.jobs[0].jobId`
  echo "[INFO] AppVeyor: downloading artifacts"
  mkdir -p $ARTIFACTDIR
  wget $BASEURL/buildjobs/$JOBID/artifacts/$ARTIFACTSRC -O $ARTIFACT
else
  echo "[ERROR] AppVeyor: build failed"
  exit 2
fi
