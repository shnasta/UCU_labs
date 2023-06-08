cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME cmake_new_lib)
project($(PROJECT_NAME) C)

set(CMAKE_C_STANDARD 99)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_executable(cmake_new_lib_cjpeg cjpeg.c cdjpeg.c)
add_executable(cmake_new_lib_djpeg djpeg.c cdjpeg.c)

target_link_libraries(cmake_new_lib_cjpeg cmake_new_lib.lib)
target_link_libraries(cmake_new_lib_djpeg cmake_new_lib.lib)

target_include_directories(cmake_new_lib_cjpeg PRIVATE ${CMAKE_SOURCE_DIR}/library)
target_include_directories(cmake_new_lib_djpeg PRIVATE ${CMAKE_SOURCE_DIR}/library)
