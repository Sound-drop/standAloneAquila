# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/micklin/Documents/cpp_final/aquila-standalone-example

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/micklin/Documents/cpp_final/aquila-standalone-example

# Include any dependencies generated for this target.
include CMakeFiles/Aquila_example.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Aquila_example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Aquila_example.dir/flags.make

CMakeFiles/Aquila_example.dir/main.cpp.o: CMakeFiles/Aquila_example.dir/flags.make
CMakeFiles/Aquila_example.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/micklin/Documents/cpp_final/aquila-standalone-example/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Aquila_example.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Aquila_example.dir/main.cpp.o -c /Users/micklin/Documents/cpp_final/aquila-standalone-example/main.cpp

CMakeFiles/Aquila_example.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Aquila_example.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/micklin/Documents/cpp_final/aquila-standalone-example/main.cpp > CMakeFiles/Aquila_example.dir/main.cpp.i

CMakeFiles/Aquila_example.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Aquila_example.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/micklin/Documents/cpp_final/aquila-standalone-example/main.cpp -o CMakeFiles/Aquila_example.dir/main.cpp.s

CMakeFiles/Aquila_example.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Aquila_example.dir/main.cpp.o.requires

CMakeFiles/Aquila_example.dir/main.cpp.o.provides: CMakeFiles/Aquila_example.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Aquila_example.dir/build.make CMakeFiles/Aquila_example.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Aquila_example.dir/main.cpp.o.provides

CMakeFiles/Aquila_example.dir/main.cpp.o.provides.build: CMakeFiles/Aquila_example.dir/main.cpp.o


# Object files for target Aquila_example
Aquila_example_OBJECTS = \
"CMakeFiles/Aquila_example.dir/main.cpp.o"

# External object files for target Aquila_example
Aquila_example_EXTERNAL_OBJECTS =

Aquila_example: CMakeFiles/Aquila_example.dir/main.cpp.o
Aquila_example: CMakeFiles/Aquila_example.dir/build.make
Aquila_example: CMakeFiles/Aquila_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/micklin/Documents/cpp_final/aquila-standalone-example/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Aquila_example"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Aquila_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Aquila_example.dir/build: Aquila_example

.PHONY : CMakeFiles/Aquila_example.dir/build

CMakeFiles/Aquila_example.dir/requires: CMakeFiles/Aquila_example.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Aquila_example.dir/requires

CMakeFiles/Aquila_example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Aquila_example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Aquila_example.dir/clean

CMakeFiles/Aquila_example.dir/depend:
	cd /Users/micklin/Documents/cpp_final/aquila-standalone-example && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/micklin/Documents/cpp_final/aquila-standalone-example /Users/micklin/Documents/cpp_final/aquila-standalone-example /Users/micklin/Documents/cpp_final/aquila-standalone-example /Users/micklin/Documents/cpp_final/aquila-standalone-example /Users/micklin/Documents/cpp_final/aquila-standalone-example/CMakeFiles/Aquila_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Aquila_example.dir/depend

