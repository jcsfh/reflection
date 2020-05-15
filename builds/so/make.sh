#!/bin/sh

echo "start to build..."

cd ../../src/runtime/Release
make clean
make
cp -r libgcrt.so ../../../builds/so

cd ../../reflect/Release
make clean
make
cp -r libreflect.so ../../../builds/so

cd ../../../builds/so
echo "build complete"