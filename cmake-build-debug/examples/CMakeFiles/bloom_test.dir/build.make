# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/thanasis/CLionProjects/Log-Structured-Merge-tree

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug

# Include any dependencies generated for this target.
include examples/CMakeFiles/bloom_test.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/bloom_test.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/bloom_test.dir/flags.make

examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.o: examples/CMakeFiles/bloom_test.dir/flags.make
examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.o: ../examples/bloom_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.o"
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bloom_test.dir/bloom_test.cpp.o -c /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/examples/bloom_test.cpp

examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bloom_test.dir/bloom_test.cpp.i"
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/examples/bloom_test.cpp > CMakeFiles/bloom_test.dir/bloom_test.cpp.i

examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bloom_test.dir/bloom_test.cpp.s"
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/examples/bloom_test.cpp -o CMakeFiles/bloom_test.dir/bloom_test.cpp.s

# Object files for target bloom_test
bloom_test_OBJECTS = \
"CMakeFiles/bloom_test.dir/bloom_test.cpp.o"

# External object files for target bloom_test
bloom_test_EXTERNAL_OBJECTS =

examples/bloom_test: examples/CMakeFiles/bloom_test.dir/bloom_test.cpp.o
examples/bloom_test: examples/CMakeFiles/bloom_test.dir/build.make
examples/bloom_test: examples/CMakeFiles/bloom_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bloom_test"
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bloom_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/bloom_test.dir/build: examples/bloom_test

.PHONY : examples/CMakeFiles/bloom_test.dir/build

examples/CMakeFiles/bloom_test.dir/clean:
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples && $(CMAKE_COMMAND) -P CMakeFiles/bloom_test.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/bloom_test.dir/clean

examples/CMakeFiles/bloom_test.dir/depend:
	cd /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thanasis/CLionProjects/Log-Structured-Merge-tree /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/examples /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples /Users/thanasis/CLionProjects/Log-Structured-Merge-tree/cmake-build-debug/examples/CMakeFiles/bloom_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/bloom_test.dir/depend
