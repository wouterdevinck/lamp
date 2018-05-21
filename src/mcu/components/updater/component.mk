COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_EMBED_FILES := ../../../../build/fpga/lamp.bin
COMPONENT_EXTRA_CLEAN := version.h

version.h:
	printf "#pragma once\n#define FPGA_HASH \"%s\"\n#define LAMP_VERSION \"%s\"\n" \
	  `cat ../../../build/fpga/lamp.hash` \
	  `git describe --tags --dirty` > $@

Updater.o: version.h