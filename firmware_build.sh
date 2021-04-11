#!/bin/sh

mkdir -p build/firmware
pushd build/firmware
cmake -DPROJECT_SOURCE_DIRECTORY="../.." -DCMAKE_TOOLCHAIN_FILE="../toolchains/arm.toolchain.cmake" ../../code && make
popd
