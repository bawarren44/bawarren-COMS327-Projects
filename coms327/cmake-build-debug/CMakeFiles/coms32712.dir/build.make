# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /cygdrive/c/Users/sup3r/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/sup3r/.CLion2019.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/sup3r/coms327/coms327

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/coms32712.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/coms32712.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/coms32712.dir/flags.make

CMakeFiles/coms32712.dir/termboxx.c.o: CMakeFiles/coms32712.dir/flags.make
CMakeFiles/coms32712.dir/termboxx.c.o: ../termboxx.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/coms32712.dir/termboxx.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/coms32712.dir/termboxx.c.o   -c /cygdrive/c/Users/sup3r/coms327/coms327/termboxx.c

CMakeFiles/coms32712.dir/termboxx.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/coms32712.dir/termboxx.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/sup3r/coms327/coms327/termboxx.c > CMakeFiles/coms32712.dir/termboxx.c.i

CMakeFiles/coms32712.dir/termboxx.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/coms32712.dir/termboxx.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/sup3r/coms327/coms327/termboxx.c -o CMakeFiles/coms32712.dir/termboxx.c.s

# Object files for target coms32712
coms32712_OBJECTS = \
"CMakeFiles/coms32712.dir/termboxx.c.o"

# External object files for target coms32712
coms32712_EXTERNAL_OBJECTS =

coms32712.exe: CMakeFiles/coms32712.dir/termboxx.c.o
coms32712.exe: CMakeFiles/coms32712.dir/build.make
coms32712.exe: CMakeFiles/coms32712.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable coms32712.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/coms32712.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/coms32712.dir/build: coms32712.exe

.PHONY : CMakeFiles/coms32712.dir/build

CMakeFiles/coms32712.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/coms32712.dir/cmake_clean.cmake
.PHONY : CMakeFiles/coms32712.dir/clean

CMakeFiles/coms32712.dir/depend:
	cd /cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/sup3r/coms327/coms327 /cygdrive/c/Users/sup3r/coms327/coms327 /cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug /cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug /cygdrive/c/Users/sup3r/coms327/coms327/cmake-build-debug/CMakeFiles/coms32712.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/coms32712.dir/depend
