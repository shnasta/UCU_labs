cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME mystring)
project($(PROJECT_NAME) CXX)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

set(MY_LIB mystring.cpp mystring.h)
add_library(mystring_shared SHARED ${MY_LIB})
