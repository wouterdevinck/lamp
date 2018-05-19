#!/bin/bash
set -ex

mkdir -p ~/fpga

cd ~/fpga
git clone https://github.com/cliffordwolf/icestorm.git icestorm
cd icestorm
git checkout 237280ce44f72c7b2e1ca671d5113dba34cc4fca
make -j$(nproc)
make install # because chipdb needed by arachne
make DESTDIR=~/fpga/out install

cd ~/fpga
git clone https://github.com/cseed/arachne-pnr.git arachne-pnr
cd arachne-pnr
git checkout ea2d04215bc0fd6072cda244caeb6670892033b3
make -j$(nproc)
make DESTDIR=~/fpga/out install

cd ~/fpga
git clone https://github.com/YosysHQ/yosys.git yosys
cd yosys
git checkout 24e64016173f586233e77f4e0d70c7883f98d8f7
make -j$(nproc)
make DESTDIR=~/fpga/out install

cd ~/fpga
git clone http://git.veripool.org/git/verilator verilator
cd verilator
git checkout verilator_3_922
autoconf
./configure --prefix=`echo ~/fpga/out`
make
make install

cd ~/fpga
git clone https://github.com/steveicarus/iverilog.git iverilog
cd iverilog
git checkout v10_2
autoconf
./configure --prefix=`echo ~/fpga/out`
make
make install