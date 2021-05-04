#!/bin/sh

mkdir -p build/unittests
pushd build/unittests
cmake ../../unittests && make && ctest -V
popd
