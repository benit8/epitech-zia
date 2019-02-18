#!/bin/sh

rm -rf ./build
rm ./modules/*.so

if [ -f ./zia ]; then
	rm ./zia
fi

mkdir ./build && cd ./build &&
conan install .. --build=missing &&
cmake .. -G "Unix Makefiles" && cmake --build . &&
make &&

# we still in ./build
cp ./lib/*.so ../modules/ &&
cp ./bin/zia ../