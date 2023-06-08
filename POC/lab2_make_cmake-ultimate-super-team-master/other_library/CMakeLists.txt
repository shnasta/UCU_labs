cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME cmake_new_lib)
project($(PROJECT_NAME) C)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_subdirectory(library)
add_subdirectory(example)
