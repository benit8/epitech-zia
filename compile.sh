#!/bin/sh


if [ $# -ge 1 ] && [ "$1" == "ssl" ]; then
	cd ./modules/SSL/
	cmake . -G "Unix Makefiles" && cmake --build . && make && mv ./libmod_ssl.so ../
	exit
fi

# does `./build` directory exists ?
if [ -d ./build ]; then
	rm -rf ./build
fi

# does any module exists ?
if ls modules/*.so 1> /dev/null 2>&1; then
	rm ./modules/*.so
fi

# does the zia bin exists ?
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
