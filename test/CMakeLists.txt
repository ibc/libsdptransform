include_directories(${sdptransform_SOURCE_DIR}/include)
include_directories(${sdptransform_SOURCE_DIR}/test/include)

set(
	SOURCE_FILES
	tests.cpp
	parse.test.cpp
	include/helpers.hpp
	include/catch.hpp
)

add_executable(test_sdptransform ${SOURCE_FILES})

target_link_libraries(test_sdptransform sdptransform)
