cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME mystring)
project($(PROJECT_NAME) CXX)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_subdirectory(library)
add_subdirectory(example)
