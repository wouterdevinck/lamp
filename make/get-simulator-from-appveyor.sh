#!/bin/bash

# TODO get from arguments and check
TOKEN=xx
VERSION=0.1.0-5-g9675350

BASEURL=https://ci.appveyor.com/api
PROJECT=wouterdevinck/lamp
ARTIFACTSRC=test%2Fsimulator%2FLamp.Simulator%2Fbin%2Fsimulator.zip
ARTIFACT=simulator.zip

# TODO: while null (with timeout?), then check for success or fail 

RESPONSE=`curl -H "Authorization: Bearer $TOKEN" -H "Accept: application/json" -X GET $BASEURL/projects/$PROJECT/build/$VERSION`

echo $RESPONSE | jq -r .build.status

echo $RESPONSE | jq -r .build.jobs[0].jobId`

wget $BASEURL/buildjobs/$JOBID/artifacts/$ARTIFACTSRC -O $ARTIFACT