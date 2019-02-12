##
## EPITECH PROJECT, 2019
## zia
## File description:
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

########################################

project(Zia)
conan_basic_setup()
add_definitions("-fPIC")
add_definitions("-W -Wall -Wextra")

########################################

set(EXEC_NAME zia)

set(SOURCES
	src/main.cpp
	src/Zia.hpp
	src/Zia.cpp
	src/Config/INIConfig.hpp
	src/Config/INIConfig.cpp
	src/Network/IpAddress.hpp
	src/Network/Packet.hpp
	src/Network/Socket.hpp
	src/Network/SocketSelector.hpp
	src/Network/TcpListener.hpp
	src/Network/TcpSocket.hpp
	src/Network/UdpSocket.hpp
	src/Network/IpAddress.cpp
	src/Network/Packet.cpp
	src/Network/Socket.cpp
	src/Network/SocketImpl.hpp
	src/Network/SocketSelector.cpp
	src/Network/TcpListener.cpp
	src/Network/TcpSocket.cpp
	src/Network/UdpSocket.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_include_directories(${EXEC_NAME} PUBLIC src/)

########################################

add_subdirectory(modules)