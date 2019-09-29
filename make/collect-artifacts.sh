#!/bin/bash
set -ex

OUT_DIR="out"
BLD_DIR="build"

rm -rf out

mkdir -p $OUT_DIR
mkdir -p $OUT_DIR/factory
mkdir -p $OUT_DIR/tool
mkdir -p $OUT_DIR/ota
mkdir -p $OUT_DIR/test

cp $BLD_DIR/fpga/lamp.bin $OUT_DIR/factory/fpga.bin
cp $BLD_DIR/mcu/lamp.bin $OUT_DIR/factory/lamp.bin
cp $BLD_DIR/mcu/partitions.bin $OUT_DIR/factory/partitions.bin
cp $BLD_DIR/mcu/ota_data_initial.bin $OUT_DIR/factory/ota_data_initial.bin
cp $BLD_DIR/mcu/phy_init_data.bin $OUT_DIR/factory/phy_init_data.bin
cp $BLD_DIR/mcu/bootloader/bootloader.bin $OUT_DIR/factory/bootloader.bin
cp $BLD_DIR/mcu-basic/lamp.hex $OUT_DIR/factory/lamp.hex

cp $BLD_DIR/tool/factory $OUT_DIR/tool/factory

cp $BLD_DIR/mcu/lamp.bin $OUT_DIR/ota/lamp.bin

cp $BLD_DIR/fpga/lamp.vcd $OUT_DIR/test/simulation.vcd