cmake_minimum_required(VERSION 2.8)

project(sdptransform)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
	SOURCE_FILES
	${sdptransform_SOURCE_DIR}/src/grammar.cpp
	${sdptransform_SOURCE_DIR}/src/parser.cpp
	${sdptransform_SOURCE_DIR}/src/writer.cpp
	${sdptransform_SOURCE_DIR}/include/sdptransform.hpp
	${sdptransform_SOURCE_DIR}/include/json.hpp
)

include_directories(${sdptransform_SOURCE_DIR}/include)

add_library(sdptransform STATIC ${SOURCE_FILES})

subdirs(test)
