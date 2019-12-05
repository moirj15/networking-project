# networking-project: Flywheel Implementation

This is an implementation of Google's Flywheel server as described in the paper 
*Flywheel: Google’s Data Compression Proxy for the Mobile Web*.

To build it you need the following dependencies installed:
* ZLib
* libwebp

You will also need a C++ compiler that implements C++17's filesystem library and CMake.

To build:
    `mkdir build`
    `cd build`
    `cmake ..`
    `make`
