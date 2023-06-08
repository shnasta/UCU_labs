#!/bin/bash

cxx=gcc
cxxflags=-w
lib_path=./other_library/library
example_path=./other_library/example
compiled_dir=./compiled_files_jpeg
compiled_library=$compiled_dir/compiled_library
dependencies=("${lib_path}"/*.c)
dependencies=("${dependencies[@]##*/}")

mkdir -p $compiled_dir
mkdir -p $compiled_library

for file in "${dependencies[@]}"; do
    $cxx $cxxflags -c $lib_path/"$file" -fPIC -o $compiled_library/"${file%.*}".o
done

#static library
ar rcs $compiled_library/libjpeg.a $compiled_library/*.o
#dynamic library
$cxx -shared -fPIC -o $compiled_library/libjpeg.so $compiled_library/*.o



file1=cdjpeg
executable2=cjpeg
executable3=djpeg
compiled_example=$compiled_dir/compiled_example

mkdir -p $compiled_example

$cxx $cxxflags -c $example_path/$file1.c -I$lib_path -o $compiled_example/$file1.o
$cxx $cxxflags -c $example_path/$executable2.c -I$lib_path -o $compiled_example/$executable2.o
$cxx $cxxflags -c $example_path/$executable3.c -I$lib_path -o $compiled_example/$executable3.o

$cxx $compiled_example/$executable2.o $compiled_library/libjpeg.so $compiled_example/$file1.o -o $compiled_example/$executable2
$cxx $compiled_example/$executable3.o $compiled_library/libjpeg.so $compiled_example/$file1.o -o $compiled_example/$executable3
