#!/bin/bash

rm -rf m4
rm -rf mpfr
rm -rf gmp
rm -rf mpfrc++

rm -rf gmp-6-0.0 mpfr-3.1.3 mpfrc++-3.6.2 m4-1.4.1

tar -xvf gmp-6.0.0a.tar.lz
tar -xvf mpfr-3.1.3.tar.xz
tar -xvf m4-1.4.1.tar.gz
unzip mpfrc++-3.6.2.zip -d mpfrc++-3.6.2
mkdir -p mpfrc++/include

cp mpfrc++-3.6.2/mpreal.h mpfrc++/include
