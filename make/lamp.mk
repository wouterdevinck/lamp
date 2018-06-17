DOCKER_PREFIX := wouterdevinck/lamp-buildenv-
SCRIPT_DIR := make
BUILD_DIR := build
ARTIFACT_DIR := out

MAKE_IN_DOCKER := $(MAKE) docker

.PHONY: all
all:
	$(MAKE) fpga
	$(MAKE) mcu
	$(MAKE) collect

$(BUILD_DIR)/fpga/lamp.bin:
	$(MAKE) fpga

$(BUILD_DIR)/fpga/lamp.hash:
	$(MAKE) fpga

.PHONY: fpga
fpga:
	$(MAKE_IN_DOCKER) fpga lint
	$(MAKE_IN_DOCKER) fpga simulate
	$(MAKE_IN_DOCKER) fpga hash
	$(MAKE_IN_DOCKER) fpga

.PHONY: mcu
mcu: $(BUILD_DIR)/fpga/lamp.bin $(BUILD_DIR)/fpga/lamp.hash
	$(MAKE_IN_DOCKER) mcu defconfig
	$(MAKE_IN_DOCKER) mcu

.PHONY: collect
collect:
	$(SCRIPT_DIR)/collect-artifacts.sh

.PHONY: clean
clean:
	$(MAKE_IN_DOCKER) fpga clean
	$(MAKE_IN_DOCKER) mcu clean
	$(MAKE_IN_DOCKER) mcu clean-dir
	rm -rf $(BUILD_DIR)
	rm -rf $(ARTIFACT_DIR)

.PHONY: flash-mcu
flash-mcu:
	$(IDF_PATH)/components/esptool_py/esptool/esptool.py \
	  --chip esp32 --port /dev/ttyUSB1 --baud 115200 \
	  --before default_reset --after hard_reset write_flash -z \
	  --flash_mode dio --flash_freq 40m --flash_size detect \
	  0x1000 $(ARTIFACT_DIR)/factory/bootloader.bin \
	  0x10000 $(ARTIFACT_DIR)/factory/lamp.bin \
	  0x8000 $(ARTIFACT_DIR)/factory/partitions.bin

.PHONY: menuconfig
menuconfig:
	docker run -it --rm -v $(PWD):/vol:Z -w /vol $(DOCKER_PREFIX)mcu $(MAKE) -f $(SCRIPT_DIR)/mcu.mk menuconfig

.PHONY: shell
shell:
	docker run -it --rm -v $(PWD):/vol:Z -w /vol $(DOCKER_PREFIX)$(CMDARG1) /bin/bash

.PHONY: docker
docker:
	docker run --rm -v $(PWD):/vol:Z -w /vol $(DOCKER_PREFIX)$(CMDARG1) $(MAKE) -f $(SCRIPT_DIR)/$(CMDARG1).mk $(filter-out $(CMDARG1), $(CMDARGS))

CMDS := docker shell
CMD := $(firstword $(MAKECMDGOALS))
ifeq ($(CMD),$(filter $(CMD),$(CMDS)))
  CMDARGS := $(filter-out $(CMDS), $(MAKECMDGOALS))
  CMDARG1 := $(word 2,$(MAKECMDGOALS))
  $(eval $(CMDARGS):;@:)
endif

docker-images:
	docker-compose -f $(SCRIPT_DIR)/docker-compose.yml build

docker-push:
	docker login
	docker-compose -f $(SCRIPT_DIR)/docker-compose.yml push

docker-clean:
	docker system prune -f