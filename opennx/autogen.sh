#! /bin/sh

autoreconf -fiv
./configure --prefix=/usr $*
