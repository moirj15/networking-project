cmake_minimum_required(VERSION 3.10)
project(flywheel-server)
file(GLOB SRC_FILES "src/*")
set(SOURCE_LIST 
	${SRC_FILES}
)
add_executable(
    flywheel-server
    ${SOURCE_LIST}
)
if (WIN32)
	include_directories(
        D:/libraries/zlib/include
        D:/libraries/libwebp-0.4.1-rc1-windows-x64/include
    )
	target_link_libraries(
        flywheel-server
        D:/libraries/zlib/link/zlibstat.lib
        D:/libraries/libwebp-0.4.1-rc1-windows-x64/lib/libwebp.lib
    )
  set(CMAKE_CXX_FLAGS "/std:c++17 /MP /GS /TP /W3 -D_CRT_SECURE_NO_WARNINGS")
else()
  set(CMAKE_CXX_FLAGS "-std=c++17 -g -Wall -Wextra -pedantic -D_DEBUG")
	target_link_libraries(
        flywheel-server
        pthread
        webp
        stdc++fs
    )
endif ()

