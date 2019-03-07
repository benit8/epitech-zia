#!/bin/sh


if [ "$1" == "ssl" ]; then
    cd ./modules/SSL/
    cmake . -G "Unix Makefiles" && cmake --build . && make
    mv ./libmod_ssl.so ../
    exit
fi

rm -rf ./build
rm ./modules/*.so

if [ -f ./zia ]; then
	rm ./zia
fi

mkdir ./build && cd ./build &&
conan install .. --build=missing &&
cmake .. -G "Unix Makefiles" &&
cmake --build . --config Debug -- -j 5 &&
make &&

# we still in ./build
cp ./lib/*.so ../modules/ &&
cp ./bin/zia ../
