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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build

# Include any dependencies generated for this target.
include src/CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/main.dir/flags.make

src/CMakeFiles/main.dir/queen-safety-graphics.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/queen-safety-graphics.cpp.o: ../src/queen-safety-graphics.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/main.dir/queen-safety-graphics.cpp.o"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/queen-safety-graphics.cpp.o -c /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety-graphics.cpp

src/CMakeFiles/main.dir/queen-safety-graphics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/queen-safety-graphics.cpp.i"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety-graphics.cpp > CMakeFiles/main.dir/queen-safety-graphics.cpp.i

src/CMakeFiles/main.dir/queen-safety-graphics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/queen-safety-graphics.cpp.s"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety-graphics.cpp -o CMakeFiles/main.dir/queen-safety-graphics.cpp.s

src/CMakeFiles/main.dir/queen-safety.cpp.o: src/CMakeFiles/main.dir/flags.make
src/CMakeFiles/main.dir/queen-safety.cpp.o: ../src/queen-safety.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/main.dir/queen-safety.cpp.o"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/queen-safety.cpp.o -c /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety.cpp

src/CMakeFiles/main.dir/queen-safety.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/queen-safety.cpp.i"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety.cpp > CMakeFiles/main.dir/queen-safety.cpp.i

src/CMakeFiles/main.dir/queen-safety.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/queen-safety.cpp.s"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src/queen-safety.cpp -o CMakeFiles/main.dir/queen-safety.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/queen-safety-graphics.cpp.o" \
"CMakeFiles/main.dir/queen-safety.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

src/main: src/CMakeFiles/main.dir/queen-safety-graphics.cpp.o
src/main: src/CMakeFiles/main.dir/queen-safety.cpp.o
src/main: src/CMakeFiles/main.dir/build.make
src/main: src/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable main"
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/main.dir/build: src/main

.PHONY : src/CMakeFiles/main.dir/build

src/CMakeFiles/main.dir/clean:
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/main.dir/clean

src/CMakeFiles/main.dir/depend:
	cd /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/src /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src /Users/ketang/stanfordcs106x/01-intro-code/01-intro-code/queen-safety/build/src/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/main.dir/depend

