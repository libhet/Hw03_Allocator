cmake_minimum_required(VERSION 3.9)
project(allocator)

set(CMAKE_CXX_STANDARD 14)

add_executable(allocator src/main.cpp src/my_allocator.h src/my_newdelete.h src/my_newdelete.cpp src/my_list.h)

add_subdirectory(tests)