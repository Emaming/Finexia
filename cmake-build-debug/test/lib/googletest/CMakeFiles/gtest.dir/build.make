# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/291/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /snap/clion/291/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/ClionProjects/FinexiaTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/ClionProjects/FinexiaTest/cmake-build-debug

# Include any dependencies generated for this target.
include test/lib/googletest/CMakeFiles/gtest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/lib/googletest/CMakeFiles/gtest.dir/compiler_depend.make

# Include the progress variables for this target.
include test/lib/googletest/CMakeFiles/gtest.dir/progress.make

# Include the compile flags for this target's objects.
include test/lib/googletest/CMakeFiles/gtest.dir/flags.make

test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: test/lib/googletest/CMakeFiles/gtest.dir/flags.make
test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: /home/user/ClionProjects/FinexiaTest/test/lib/googletest/src/gtest-all.cc
test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o: test/lib/googletest/CMakeFiles/gtest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/user/ClionProjects/FinexiaTest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o"
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o -MF CMakeFiles/gtest.dir/src/gtest-all.cc.o.d -o CMakeFiles/gtest.dir/src/gtest-all.cc.o -c /home/user/ClionProjects/FinexiaTest/test/lib/googletest/src/gtest-all.cc

test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/gtest.dir/src/gtest-all.cc.i"
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/ClionProjects/FinexiaTest/test/lib/googletest/src/gtest-all.cc > CMakeFiles/gtest.dir/src/gtest-all.cc.i

test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/gtest.dir/src/gtest-all.cc.s"
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/ClionProjects/FinexiaTest/test/lib/googletest/src/gtest-all.cc -o CMakeFiles/gtest.dir/src/gtest-all.cc.s

# Object files for target gtest
gtest_OBJECTS = \
"CMakeFiles/gtest.dir/src/gtest-all.cc.o"

# External object files for target gtest
gtest_EXTERNAL_OBJECTS =

lib/libgtest.a: test/lib/googletest/CMakeFiles/gtest.dir/src/gtest-all.cc.o
lib/libgtest.a: test/lib/googletest/CMakeFiles/gtest.dir/build.make
lib/libgtest.a: test/lib/googletest/CMakeFiles/gtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/user/ClionProjects/FinexiaTest/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../lib/libgtest.a"
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean_target.cmake
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/lib/googletest/CMakeFiles/gtest.dir/build: lib/libgtest.a
.PHONY : test/lib/googletest/CMakeFiles/gtest.dir/build

test/lib/googletest/CMakeFiles/gtest.dir/clean:
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest && $(CMAKE_COMMAND) -P CMakeFiles/gtest.dir/cmake_clean.cmake
.PHONY : test/lib/googletest/CMakeFiles/gtest.dir/clean

test/lib/googletest/CMakeFiles/gtest.dir/depend:
	cd /home/user/ClionProjects/FinexiaTest/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/ClionProjects/FinexiaTest /home/user/ClionProjects/FinexiaTest/test/lib/googletest /home/user/ClionProjects/FinexiaTest/cmake-build-debug /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest /home/user/ClionProjects/FinexiaTest/cmake-build-debug/test/lib/googletest/CMakeFiles/gtest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : test/lib/googletest/CMakeFiles/gtest.dir/depend

