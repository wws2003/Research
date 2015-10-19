#/bin/sh

PWD=$(pwd)
M4_DIR=$PWD/m4-1.4.1
GMP_DIR=$PWD/gmp-6.0.0
MPFR_DIR=$PWD/mpfr-3.1.3

GMP_OUT_DIR=$PWD/gmp
MPFR_OUT_DIR=$PWD/mpfr
M4_OUT_DIR=$PWD/m4

case "$1" in 
	m4)
		cd $M4_DIR
		./configure --prefix=$M4_OUT_DIR --exec-prefix=$M4_OUT_DIR		
		cd $PWD ;;
	gmp)
		cd $GMP_DIR
		./configure --prefix=$GMP_OUT_DIR --exec-prefix=$GMP_OUT_DIR --enable-cxx M4=$M4_OUT_DIR/bin/m4		
		cd $PWD ;;
	mpfr)
		cd $MPFR_DIR
		./configure --prefix=$MPFR_OUT_DIR --exec-prefix=$MPFR_OUT_DIR --with-gmp-include=$GMP_OUT_DIR/include --with-gmp-lib=$GMP_OUT_DIR/lib 
		cd $PWD ;;
	
esac
