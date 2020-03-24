# run "cmake -DCMAKE_BUILD_TYPE=Debug" to activate DEBUG macro
cmake_minimum_required(VERSION 3.10)
project(templatedb VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

set(templatedb_lib
        src/Database.cpp
        src/Operation.cpp
        src/Buffer.cpp
        src/Level.cpp
        src/LSM_T.cpp
        src/Run.cpp
        src/Tuple.cpp
        src/FencePointer.cpp)


set(bloomfilter_lib
	src/BloomFilter.cpp
	src/Murmurhash.cpp)


add_library(templatedb ${templatedb_lib})
target_include_directories(templatedb PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_library(BF ${bloomfilter_lib})
target_include_directories(BF PUBLIC ${PROJECT_SOURCE_DIR}/src)

include_directories(external)
include_directories(include)

configure_file(external/CMakeLists.txt.in external/googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/external/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/external/googletest-src"
                 "${CMAKE_BINARY_DIR}/external/googletest-build"
)

enable_testing()

add_subdirectory(examples)
add_subdirectory(tests)

