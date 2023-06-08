cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME lab1_cstring-ultimate-super-team)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} mystring.h mystring.cpp demonstration.cpp)
