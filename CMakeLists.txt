##
## EPITECH PROJECT
## Zia
## File Description:
## CMakeLists
##

cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include_directories(${CMAKE_INCLUDE_PATH})

project(Zia)
add_definitions("-fPIC")
conan_basic_setup()
set(SFML_STATIC_LIBRARIES TRUE)
set(Boost_STATIC_LIBRARIES TRUE)
find_package(SFML COMPONENTS network graphics window system REQUIRED)
find_package(Boost COMPONENTS)

set(SERVER_DIR server/)
add_subdirectory(${SERVER_DIR})

set(MODULE_DIR module/)
add_subdirectory(${MODULE_DIR})