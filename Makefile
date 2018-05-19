PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include make/lamp.mk