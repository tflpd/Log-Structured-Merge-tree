add_executable(simple_benchmark simple_benchmark.cpp)
target_link_libraries(simple_benchmark PUBLIC templatedb)

add_executable(bloom_test bloom_test.cpp)
target_link_libraries(bloom_test PUBLIC BF)
