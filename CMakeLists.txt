cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

add_library(catch INTERFACE)
target_sources(catch INTERFACE ${CMAKE_SOURCE_DIR}/catch.hpp)

add_library(polynomial INTERFACE)
target_sources(polynomial INTERFACE ${CMAKE_SOURCE_DIR}/Polynomial.hpp)

add_executable(tests tests.cpp)
target_link_libraries(tests catch polynomial)