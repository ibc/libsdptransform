cmake_minimum_required(VERSION 3.0)

project(sdptransform VERSION 1.2.10)

# For CMake >= 3.1.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# For CMake < 3.1.
add_compile_options(-std=c++14)

add_subdirectory(test readme-helper)

include_directories(${sdptransform_SOURCE_DIR}/include)

set(
	SOURCE_FILES
	src/grammar.cpp
	src/parser.cpp
	src/writer.cpp
)

# Only required for installation step
set(
	HEADER_FILES
	include/sdptransform.hpp
	include/json.hpp
)

add_library(sdptransform STATIC ${SOURCE_FILES})

install(TARGETS sdptransform DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/sdptransform)
