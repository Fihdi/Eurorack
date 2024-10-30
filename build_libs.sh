#!/bin/bash

START_DIR=$PWD
LIBDAISY_DIR=$PWD/libDaisy

echo "building libDaisy . . ."
cd "$LIBDAISY_DIR" ; make -s clean ; make -j -s
if [ $? -ne 0 ]
then
    echo "Failed to compile libDaisy"
    exit 1
fi
echo "done."
