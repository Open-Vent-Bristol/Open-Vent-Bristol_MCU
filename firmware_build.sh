#!/bin/sh

BUILD_DIR=$(pwd)/build/firmware

mkdir -p $BUILD_DIR/lib

pushd submodules/libusb_stm32
make cmsis
make module MODULE=$BUILD_DIR/lib/libusb_stm32.a MCU=stm32l476xg DEFINES="STM32L4 STM32L476xx USBD_SOF_DISABLED" CFLAGS="-mcpu=cortex-m4 -mfloat-abi=soft"
popd
pushd $BUILD_DIR
cmake -DPROJECT_SOURCE_DIRECTORY="../.." -DCMAKE_TOOLCHAIN_FILE="../toolchains/arm.toolchain.cmake" ../../code && make
popd
