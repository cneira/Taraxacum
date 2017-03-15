#!/bin/sh
set -ex
git clone https://github.com/oktal/pistache.git
cd pistache && mkdir build && cd build && cmake .. / && make && sudo make install