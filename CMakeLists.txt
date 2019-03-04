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
	common/HTTP/FieldContainer.hpp
	common/HTTP/FieldContainer.cpp
	common/HTTP/ProcessingList.hpp
	common/HTTP/ProcessingList.cpp
	common/HTTP/Request.hpp
	common/HTTP/Request.cpp
	common/HTTP/Response.hpp
	common/HTTP/Response.cpp
	common/Network/IpAddress.hpp
	common/Network/Packet.hpp
	common/Network/Socket.hpp
	common/Network/SocketImpl.hpp
	common/Network/SocketSelector.hpp
	common/Network/TcpListener.hpp
	common/Network/TcpSocket.hpp
	common/Network/UdpSocket.hpp
	common/Export.hpp
	common/HTTP.hpp
	common/Module.hpp
	common/NonCopyable.hpp
	common/json.hpp

	src/main.cpp
	src/Zia.hpp
	src/Zia.cpp
	src/ModuleLoader.hpp
	src/ModuleLoader.cpp
	src/Network/IpAddress.cpp
	src/Network/Packet.cpp
	src/Network/Socket.cpp
	src/Network/SocketSelector.cpp
	src/Network/TcpListener.cpp
	src/Network/TcpSocket.cpp
	src/Network/UdpSocket.cpp
)

add_executable(${EXEC_NAME} ${SOURCES})
target_include_directories(${EXEC_NAME} PUBLIC src/ common/)
target_link_libraries(${EXEC_NAME} dl)

########################################

add_subdirectory(modules/)