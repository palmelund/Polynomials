cmake_minimum_required(VERSION 3.10)
project(polynomials)

set(CMAKE_CXX_STANDARD 17)
add_library(polynomials Polynomial.cpp Polynomial.h)

project(catch)
add_executable(catch tests/catchmain.cpp tests/polynomial.cpp)
target_link_libraries(catch polynomials)