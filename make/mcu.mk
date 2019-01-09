PROJECT_NAME := lamp

PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
PROJECT_PATH := $(abspath $(PWD)../src/mcu)
BUILD_DIR_BASE := $(abspath $(PWD)../build/mcu)
BUILD_DIR_CORE := $(abspath $(PWD)../build/core)

export PWD
export PROJECT_PATH

include $(IDF_PATH)/make/project.mk

.PHONY: clean-dir
clean-dir:
	rm -rf $(BUILD_DIR_BASE)
	rm -rf $(BUILD_DIR_CORE)

.PHONY: clean-version
clean-version:
	rm -rf $(BUILD_DIR_BASE)/updater/version.h