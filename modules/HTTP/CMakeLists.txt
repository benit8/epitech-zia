##
## EPITECH PROJECT, 2019
## zia
## File description:
## CMakeLists.txt
##

set(MODULE_NAME mod_http)

add_definitions("-shared")
add_definitions("-fPIC")
add_definitions("-W -Wall -Wextra")

set(SOURCES
	HTTPModule.cpp
)

add_library(${MODULE_NAME} SHARED ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC ../../src/)