add_executable(basic_test basic_test.cpp)
add_executable(benchmark benchmark.cpp)
add_executable(lsm_man_test lsm_man_test.cpp)

target_link_libraries(basic_test PUBLIC templatedb BF gtest)
target_link_libraries(benchmark PUBLIC templatedb BF)
target_link_libraries(lsm_man_test PUBLIC templatedb BF gtest)

add_test(NAME basic_test COMMAND ${CMAKE_BINARY_DIR}/tests/basic_test)
add_test(NAME benchmark COMMAND ${CMAKE_BINARY_DIR}/tests/benchmark)
add_test(NAME lsm_man_test COMMAND ${CMAKE_BINARY_DIR}/tests/lsm_man_test)
