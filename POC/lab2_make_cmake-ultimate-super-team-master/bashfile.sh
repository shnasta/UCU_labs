#!/usr/bin/bash

mkdir -p compiled_files
g++ -c library/mystring.cpp -o compiled_files/mystring.o
g++ library/mystring.cpp -o compiled_files/mystringdynamic.o -c -fPIC
g++ -shared -fPIC -o compiled_files/libmystring.so  compiled_files/mystringdynamic.o
ar rcs compiled_files/libmystring.a compiled_files/mystring.o

g++ -c example/demonstration.cpp -o compiled_files/demonstration.o
g++ compiled_files/demonstration.o ./compiled_files/libmystring.so -o compiled_files/demo

