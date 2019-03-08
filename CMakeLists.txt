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
	src/ModuleLoader.cpp
	src/Network/IpAddress.cpp
	src/Network/Packet.cpp
	src/Network/Socket.cpp
	src/Network/SocketSelector.cpp
	src/Network/TcpListener.cpp
	src/Network/TcpSocket.cpp
	src/Network/UdpSocket.cpp
	src/HTTP/FieldContainer.cpp
	src/HTTP/Request.cpp
	src/HTTP/Response.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_include_directories(${EXEC_NAME} PUBLIC src/)
target_link_libraries(${EXEC_NAME} dl pthread)

########################################

add_subdirectory(modules/)
