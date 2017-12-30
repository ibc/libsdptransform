include_directories(${sdptransform_SOURCE_DIR}/include)

add_executable(test_sdptransform "test.cpp")

target_link_libraries(test_sdptransform sdptransform)
