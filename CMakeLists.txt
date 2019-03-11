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
add_definitions("-fPIC -g3")
add_definitions("-W -Wall -Wextra")

########################################

set(EXEC_NAME zia)

set(SOURCES
	src/main.cpp
	src/Zia.cpp
	src/Zia.hpp
	src/ModuleLoader.cpp
	src/ModuleLoader.hpp
	src/Logger.cpp
	src/Logger.hpp
	src/Network/IpAddress.cpp
	src/Network/IpAddress.hpp
	src/Network/Packet.cpp
	src/Network/Packet.hpp
	src/Network/Socket.cpp
	src/Network/Socket.hpp
	src/Network/SocketImpl.hpp
	src/Network/SocketSelector.cpp
	src/Network/SocketSelector.hpp
	src/Network/TcpListener.cpp
	src/Network/TcpListener.hpp
	src/Network/TcpSocket.cpp
	src/Network/TcpSocket.hpp
	src/Network/UdpSocket.cpp
	src/Network/UdpSocket.hpp
	src/HTTP/FieldContainer.cpp
	src/HTTP/FieldContainer.hpp
	src/HTTP/Request.cpp
	src/HTTP/Request.hpp
	src/HTTP/Response.cpp
	src/HTTP/Response.hpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_include_directories(${EXEC_NAME} PUBLIC src/)
target_link_libraries(${EXEC_NAME} dl pthread)

########################################

add_subdirectory(modules/)
