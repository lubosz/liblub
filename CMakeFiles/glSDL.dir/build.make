# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bmonkey/workspace/GFX/gl3cone

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bmonkey/workspace/GFX/gl3cone

# Include any dependencies generated for this target.
include CMakeFiles/glSDL.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glSDL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glSDL.dir/flags.make

CMakeFiles/glSDL.dir/src/main.cpp.o: CMakeFiles/glSDL.dir/flags.make
CMakeFiles/glSDL.dir/src/main.cpp.o: src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bmonkey/workspace/GFX/gl3cone/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glSDL.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glSDL.dir/src/main.cpp.o -c /home/bmonkey/workspace/GFX/gl3cone/src/main.cpp

CMakeFiles/glSDL.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glSDL.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bmonkey/workspace/GFX/gl3cone/src/main.cpp > CMakeFiles/glSDL.dir/src/main.cpp.i

CMakeFiles/glSDL.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glSDL.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bmonkey/workspace/GFX/gl3cone/src/main.cpp -o CMakeFiles/glSDL.dir/src/main.cpp.s

CMakeFiles/glSDL.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/glSDL.dir/src/main.cpp.o.requires

CMakeFiles/glSDL.dir/src/main.cpp.o.provides: CMakeFiles/glSDL.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/glSDL.dir/build.make CMakeFiles/glSDL.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/glSDL.dir/src/main.cpp.o.provides

CMakeFiles/glSDL.dir/src/main.cpp.o.provides.build: CMakeFiles/glSDL.dir/src/main.cpp.o
.PHONY : CMakeFiles/glSDL.dir/src/main.cpp.o.provides.build

CMakeFiles/glSDL.dir/src/utils.cpp.o: CMakeFiles/glSDL.dir/flags.make
CMakeFiles/glSDL.dir/src/utils.cpp.o: src/utils.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bmonkey/workspace/GFX/gl3cone/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glSDL.dir/src/utils.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glSDL.dir/src/utils.cpp.o -c /home/bmonkey/workspace/GFX/gl3cone/src/utils.cpp

CMakeFiles/glSDL.dir/src/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glSDL.dir/src/utils.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bmonkey/workspace/GFX/gl3cone/src/utils.cpp > CMakeFiles/glSDL.dir/src/utils.cpp.i

CMakeFiles/glSDL.dir/src/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glSDL.dir/src/utils.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bmonkey/workspace/GFX/gl3cone/src/utils.cpp -o CMakeFiles/glSDL.dir/src/utils.cpp.s

CMakeFiles/glSDL.dir/src/utils.cpp.o.requires:
.PHONY : CMakeFiles/glSDL.dir/src/utils.cpp.o.requires

CMakeFiles/glSDL.dir/src/utils.cpp.o.provides: CMakeFiles/glSDL.dir/src/utils.cpp.o.requires
	$(MAKE) -f CMakeFiles/glSDL.dir/build.make CMakeFiles/glSDL.dir/src/utils.cpp.o.provides.build
.PHONY : CMakeFiles/glSDL.dir/src/utils.cpp.o.provides

CMakeFiles/glSDL.dir/src/utils.cpp.o.provides.build: CMakeFiles/glSDL.dir/src/utils.cpp.o
.PHONY : CMakeFiles/glSDL.dir/src/utils.cpp.o.provides.build

# Object files for target glSDL
glSDL_OBJECTS = \
"CMakeFiles/glSDL.dir/src/main.cpp.o" \
"CMakeFiles/glSDL.dir/src/utils.cpp.o"

# External object files for target glSDL
glSDL_EXTERNAL_OBJECTS =

glSDL: CMakeFiles/glSDL.dir/src/main.cpp.o
glSDL: CMakeFiles/glSDL.dir/src/utils.cpp.o
glSDL: CMakeFiles/glSDL.dir/build.make
glSDL: CMakeFiles/glSDL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable glSDL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glSDL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glSDL.dir/build: glSDL
.PHONY : CMakeFiles/glSDL.dir/build

CMakeFiles/glSDL.dir/requires: CMakeFiles/glSDL.dir/src/main.cpp.o.requires
CMakeFiles/glSDL.dir/requires: CMakeFiles/glSDL.dir/src/utils.cpp.o.requires
.PHONY : CMakeFiles/glSDL.dir/requires

CMakeFiles/glSDL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glSDL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glSDL.dir/clean

CMakeFiles/glSDL.dir/depend:
	cd /home/bmonkey/workspace/GFX/gl3cone && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bmonkey/workspace/GFX/gl3cone /home/bmonkey/workspace/GFX/gl3cone /home/bmonkey/workspace/GFX/gl3cone /home/bmonkey/workspace/GFX/gl3cone /home/bmonkey/workspace/GFX/gl3cone/CMakeFiles/glSDL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glSDL.dir/depend

