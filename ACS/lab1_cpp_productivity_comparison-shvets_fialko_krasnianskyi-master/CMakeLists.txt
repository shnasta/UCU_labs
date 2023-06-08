cmake_minimum_required(VERSION 3.15)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(PROJECT_NAME performance)
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ../bin)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too much problem for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing.
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan)
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
set(ENABLE_TSan OFF)
#! MemorySanitizer detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)


#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

##########################################################
# Project files, packages, libraries and so on
##########################################################

#! Project main executable source compilation
add_executable(${PROJECT_NAME}_strings parse_from_file.cpp
		options_parser/options_parser.cpp options_parser/options_parser.h src/read_from_file.cpp src/read_from_file.h)

add_executable(${PROJECT_NAME}_uints parse_into_unsigned.cpp
		options_parser/options_parser.cpp options_parser/options_parser.h src/read_from_file.cpp src/read_from_file.h src/parse_from_string.cpp src/parse_from_string.h)

#! Put path to your project headers
target_include_directories(${PROJECT_NAME}_strings  PRIVATE options_parser)
target_include_directories(${PROJECT_NAME}_strings  PRIVATE src)
target_include_directories(${PROJECT_NAME}_uints  PRIVATE options_parser)
target_include_directories(${PROJECT_NAME}_uints  PRIVATE src)


#! Add external packages
# options_parser requires boost::program_options library
find_package(Boost 1.71.0 COMPONENTS program_options system REQUIRED)
target_include_directories(${PROJECT_NAME}_strings PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_strings Boost::program_options Boost::system)
target_include_directories(${PROJECT_NAME}_uints PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_uints Boost::program_options Boost::system)


##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}_strings>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

INSTALL(PROGRAMS
		$<TARGET_FILE:${PROJECT_NAME}_uints>	# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
		DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)