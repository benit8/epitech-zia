##
## EPITECH PROJECT, 2019
## zia
## File description:
## CMakeLists.txt
##

set(MODULE_NAME mod_fileserve)

add_definitions("-shared")
add_definitions("-fPIC")
add_definitions("-W -Wall -Wextra")

set(SOURCES
	FileServeModule.cpp
	FileServeModule.hpp
)

add_library(${MODULE_NAME} SHARED ${SOURCES})
target_link_libraries(${MODULE_NAME} stdc++fs)
target_include_directories(${MODULE_NAME} PUBLIC ../../src/)