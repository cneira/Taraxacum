#!/bin/sh
set -ex
git clone  https://github.com/miloyip/rapidjson.git
cd rapidjson
git submodule update --init
mkdir build
cd build
cmake ..
sudo make install
cd
git clone https://github.com/cneira/pistache
cd pistache
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
sudo make install


