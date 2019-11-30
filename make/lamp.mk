DOCKER_PREFIX := wouterdevinck/lamp-buildenv-
SCRIPT_DIR := make
BUILD_DIR := build
ARTIFACT_DIR := out
MCU_TTY := /dev/ttyUSB1

MAKE_IN_DOCKER := $(MAKE) docker

.PHONY: all
all:
	$(MAKE) fpga
	$(MAKE) mcu
	$(MAKE) mcu-basic
	$(MAKE) tool
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
	$(MAKE_IN_DOCKER) mcu clean-version
	$(MAKE_IN_DOCKER) mcu defconfig
	$(MAKE_IN_DOCKER) mcu

.PHONY: mcu-basic
mcu-basic: 
	$(MAKE_IN_DOCKER) mcu-basic

.PHONY: tool
tool: 
	$(MAKE_IN_DOCKER) tool

.PHONY: collect
collect:
	$(SCRIPT_DIR)/collect-artifacts.sh

.PHONY: clean
clean:
	$(MAKE_IN_DOCKER) fpga clean
	$(MAKE_IN_DOCKER) mcu clean
	$(MAKE_IN_DOCKER) mcu clean-dir
	$(MAKE_IN_DOCKER) mcu-basic clean
	rm -rf $(BUILD_DIR)
	rm -rf $(ARTIFACT_DIR)

# TODO move flash-mcu and monitor-mcu to specific makefile, like done for flash-mcu-basic

.PHONY: flash-mcu
flash-mcu:
	$(IDF_PATH)/components/esptool_py/esptool/esptool.py \
	  --chip esp32 --port $(MCU_TTY) --baud 115200 \
	  --before default_reset --after hard_reset write_flash -z \
	  --flash_mode dio --flash_freq 40m --flash_size detect \
	  0x1000 $(ARTIFACT_DIR)/factory/bootloader.bin \
	  0x8000 $(ARTIFACT_DIR)/factory/partitions.bin \
	  0xd000 $(ARTIFACT_DIR)/factory/ota_data_initial.bin \
	  0xf000 $(ARTIFACT_DIR)/factory/phy_init_data.bin \
	  0x10000 $(ARTIFACT_DIR)/factory/lamp.bin 

.PHONY: reset-ota-mcu
reset-ota-mcu:
	$(IDF_PATH)/components/esptool_py/esptool/esptool.py \
	  --chip esp32 --port $(MCU_TTY) --baud 115200 \
	  --after hard_reset erase_region 0xd000 0x2000

.PHONY: reset-nvs-mcu
reset-nvs-mcu:
	$(IDF_PATH)/components/esptool_py/esptool/esptool.py \
	  --chip esp32 --port $(MCU_TTY) --baud 115200 \
	  --after hard_reset erase_region 0x9000 0x4000

.PHONY: monitor-mcu
monitor-mcu:
	$(IDF_PATH)/tools/idf_monitor.py \
	  --port $(MCU_TTY) $(BUILD_DIR)/mcu/lamp.elf

.PHONY: flash-mcu-basic
flash-mcu-basic:
	$(MAKE) -f $(SCRIPT_DIR)/mcu-basic.mk flash

.PHONY: flash-fpga
flash-fpga:
	$(MAKE) -f $(SCRIPT_DIR)/fpga.mk flash

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
