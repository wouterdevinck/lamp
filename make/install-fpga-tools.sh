#!/bin/bash
set -ex

mkdir -p ~/fpga

cd ~/fpga
git clone https://github.com/YosysHQ/icestorm.git icestorm
cd icestorm
git checkout d12308775684cf43ab923227235b4ad43060015e
make -j$(nproc)
make DESTDIR=~/fpga/out PREFIX= install

cd ~/fpga
git clone https://github.com/YosysHQ/nextpnr.git nextpnr
cd nextpnr
git checkout fbe486df459909065d6852a7495a212dfd2accef
cmake -DARCH=ice40 -DBUILD_PYTHON=OFF -DBUILD_GUI=OFF -DCMAKE_INSTALL_PREFIX=`echo ~/fpga/out` .
make -j$(nproc)
make install

cd ~/fpga
git clone https://github.com/YosysHQ/yosys.git yosys
cd yosys
git checkout 04f6158bf2ce9a95970f1be435351786f59bed92
make -j$(nproc)
make DESTDIR=~/fpga/out install

cd ~/fpga
git clone https://github.com/verilator/verilator.git verilator
cd verilator
git checkout v4.038
autoconf
./configure --prefix=`echo ~/fpga/out`
make
make install

cd ~/fpga
git clone https://github.com/steveicarus/iverilog.git iverilog
cd iverilog
git checkout v10_3
autoconf
./configure --prefix=`echo ~/fpga/out`
make
make install