##
## EPITECH PROJECT
## Zia
## File Description:
## CMakeLists server
##

set(SERVER_EXE zia_server)
set(SERVER_SOURCE src)
set(SERVER_INCLUDE header)

set(SERVER_SOURCES
  ${SERVER_SOURCE}/Main.cpp
  ${SERVER_INCLUDE}/Server.hpp)

add_executable(${SERVER_EXE} ${SERVER_SOURCES})
target_include_directories(${SERVER_EXE} PUBLIC ${SERVER_INCLUDE})
target_link_libraries(${SERVER_EXE} ${CONAN_LIBS})
