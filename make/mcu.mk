PROJECT_NAME := lamp

PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRCDIR := $(abspath $(PWD)../src/mcu)
OUTDIR := $(abspath $(PWD)../build/mcu)
FPGADIR := $(abspath $(PWD)../build/fpga)

export PROJECT_NAME

.PHONY: build
build:
	$(PWD)build-mcu.sh reconfigure $(SRCDIR) $(OUTDIR)
	$(PWD)build-mcu.sh build $(SRCDIR) $(OUTDIR)

.PHONY: menuconfig
menuconfig:
	$(PWD)build-mcu.sh menuconfig $(SRCDIR) $(OUTDIR)

.PHONY: clean-dir
clean-dir:
	rm -rf $(OUTDIR)

.PHONY: clean-version
clean-version:
	rm -rf $(OUTDIR)/esp-idf/updater/version.h