#!/bin/bash
export RELEASE=$(git describe --always --tag)
export DEV=_dev
export OUT_DIR=out/$RELEASE$DEV

export DATE=`date +%Y%m%d`
export OUT_SUB_DIR=$DATE
export TIME=`date +%H%M`

export OUT_FILE=out$TIME.txt
export GIT_COMMIT=$(git log --pretty=%H -1)
export RUN_FILE=./GateApproximationApp

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib

mkdir -p $OUT_DIR/$OUT_SUB_DIR

echo $GIT_COMMIT > $OUT_DIR/$OUT_SUB_DIR/$OUT_FILE
echo $RUN_FILE $@ >> $OUT_DIR/$OUT_SUB_DIR/$OUT_FILE
$RUN_FILE $@ >> $OUT_DIR/$OUT_SUB_DIR/$OUT_FILE

less $OUT_DIR/$OUT_SUB_DIR/$OUT_FILE

