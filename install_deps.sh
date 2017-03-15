#!/bin/sh
set -ex
git clone  https://github.com/miloyip/rapidjson.git
cd rapidjson
cd include
sudo cp -rp rapidjson  /usr/include/
cd
git clone https://github.com/oktal/pistache.git
cd pistache
git submodule update --init
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install


