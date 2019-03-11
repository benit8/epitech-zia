##
## EPITECH PROJECT, 2019
## zia
## File description:
## CMakeLists.txt
##

set(MODULE_NAME mod_ssl)

add_definitions("-fPIC")
add_definitions("-shared")
add_definitions("-W -Wall -Wextra")
add_definitions("-lssl -lcrypto -ldl")
add_definitions("-L/usr/lib")

set(SOURCES
	SSLModule.cpp
	SSLModule.hpp
	Colors.h
)

add_library(${MODULE_NAME} SHARED ${SOURCES})
target_link_libraries(${MODULE_NAME} ssl crypto)
target_include_directories(${MODULE_NAME} PUBLIC ../../src/)
