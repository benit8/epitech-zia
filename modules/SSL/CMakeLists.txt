##
## EPITECH PROJECT, 2019
## zia
## File description:
## CMakeLists.txt
##

set(MODULE_NAME mod_ssl)

add_definitions("-shared")
add_definitions("-fPIC")
add_definitions("-W -Wall -Wextra -lssl -lcrypto")

set(SOURCES
	../../common/Export.hpp
	../../common/Module.hpp
	SSLModule.hpp
	SSLModule.cpp
)

add_library(${MODULE_NAME} SHARED ${SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC ../../common/)
