#!/bin/bash

SCRIPT_PATH=`realpath "$0"`
SCRIPT_DIR=`dirname "$SCRIPT_PATH"`



sudo apt-get install -y make build-essential g++ wget curl llvm clang bison flex
sudo apt-get install -y git cmake 

sudo apt-get install -y libbz2-dev libreadline-dev libsqlite3-dev 
sudo apt-get install -y libncursesw5-dev xz-utils tk-dev libxml2-dev libxmlsec1-dev
sudo apt-get install -y libffi-dev liblzma-dev libboost-all-dev libeigen3-dev
sudo apt-get install -y libftdi1-2 libftdi1-dev libhidapi-hidraw0 libhidapi-dev
sudo apt-get install -y libudev-dev pkg-config libssl-dev zlib1g-dev
sudo apt-get install -y gawk tcl-dev graphviz xdot

sudo apt-get install iverilog

# python libs
sudo apt install -y python3-pip
pip install apycula
pip install yowasp-nextpnr-gowin

mkdir -p ${SCRIPT_DIR}/LibsBuild

# yosys
cd ${SCRIPT_DIR}/LibsBuild
git clone https://github.com/YosysHQ/yosys.git
cd yosys
make -j$(nproc)
sudo make install

# nextpnr-gowin
cd ${SCRIPT_DIR}/LibsBuild
git clone https://github.com/YosysHQ/nextpnr.git
cd nextpnr
cmake . -DARCH=gowin -DGOWIN_BBA_EXECUTABLE=`which gowin_bba`
make -j$(nproc)
sudo make install

# openFPGALoader
cd ${SCRIPT_DIR}/LibsBuild
git clone https://github.com/trabucayre/openFPGALoader.git
cd openFPGALoader
mkdir build
cd build
cmake ../ 
cmake --build . -j$(nproc)
sudo make install

# Clean
sudo rm -Rf ${SCRIPT_DIR}/LibsBuild
