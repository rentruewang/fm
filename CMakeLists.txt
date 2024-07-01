cmake_minimum_required(VERSION 3.22)
project(fm)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_STANDARD 20)

file(GLOB FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(fm ${FILES})
target_include_directories(fm PRIVATE "include")
