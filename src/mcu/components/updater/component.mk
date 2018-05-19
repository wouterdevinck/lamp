COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_EMBED_FILES := ../../../../build/fpga/lamp.bin
COMPONENT_EXTRA_CLEAN := fpga_version.h

fpga_version.h:
	printf "#pragma once\n#define FPGA_HASH \"%s\"\n" `cat ../../../build/fpga/lamp.hash` > $@

Updater.o: fpga_version.h