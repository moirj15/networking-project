cmake_minimum_required(VERSION 3.10)
project(flywheel-server)
file(GLOB SRC_FILES "src/*")
set(SOURCE_LIST 
	${SRC_FILES}
)

if (WIN32)
  set(CMAKE_CXX_FLAGS "/std:c++17 /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
else()
  set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
endif ()
add_executable(
    flywheel-server
    ${SOURCE_LIST}
)
