cmake_minimum_required(VERSION 2.8)

project(sdptransform)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${sdptransform_SOURCE_DIR}/include)

set(
	SOURCE_FILES
	src/grammar.cpp
	src/parser.cpp
	src/writer.cpp
	include/sdptransform.hpp
	include/json.hpp
)

add_library(sdptransform STATIC ${SOURCE_FILES})

subdirs(test readme-helper)
