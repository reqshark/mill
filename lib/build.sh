#!/usr/bin/env sh
export MILLPATH=`pwd`/lib
cd  libmill && ./autogen.sh
./configure --prefix=$MILLPATH
make
make install
