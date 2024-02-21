cmake_minimum_required(VERSION 3.16)

project(sdptransform VERSION 1.2.10)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_UTILS "Build utility applications" OFF)

find_package(nlohmann_json 3.11.3 REQUIRED)

if (${BUILD_TESTS})
    add_subdirectory(test)
endif()

if (${BUILD_UTILS})
    add_subdirectory(utils)
endif()

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
)

add_library(sdptransform STATIC ${SOURCE_FILES})
target_link_libraries(sdptransform PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS sdptransform DESTINATION lib)
install(FILES ${HEADER_FILES} DESTINATION include/sdptransform)
