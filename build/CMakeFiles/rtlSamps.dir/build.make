# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jojo/cradio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jojo/cradio/build

# Include any dependencies generated for this target.
include CMakeFiles/rtlSamps.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rtlSamps.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtlSamps.dir/flags.make

CMakeFiles/rtlSamps.dir/rtlSamps.c.o: CMakeFiles/rtlSamps.dir/flags.make
CMakeFiles/rtlSamps.dir/rtlSamps.c.o: ../rtlSamps.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/rtlSamps.dir/rtlSamps.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rtlSamps.dir/rtlSamps.c.o   -c /home/jojo/cradio/rtlSamps.c

CMakeFiles/rtlSamps.dir/rtlSamps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rtlSamps.dir/rtlSamps.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jojo/cradio/rtlSamps.c > CMakeFiles/rtlSamps.dir/rtlSamps.c.i

CMakeFiles/rtlSamps.dir/rtlSamps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rtlSamps.dir/rtlSamps.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jojo/cradio/rtlSamps.c -o CMakeFiles/rtlSamps.dir/rtlSamps.c.s

CMakeFiles/rtlSamps.dir/getSamps.c.o: CMakeFiles/rtlSamps.dir/flags.make
CMakeFiles/rtlSamps.dir/getSamps.c.o: ../getSamps.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/rtlSamps.dir/getSamps.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/rtlSamps.dir/getSamps.c.o   -c /home/jojo/cradio/getSamps.c

CMakeFiles/rtlSamps.dir/getSamps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/rtlSamps.dir/getSamps.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jojo/cradio/getSamps.c > CMakeFiles/rtlSamps.dir/getSamps.c.i

CMakeFiles/rtlSamps.dir/getSamps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/rtlSamps.dir/getSamps.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jojo/cradio/getSamps.c -o CMakeFiles/rtlSamps.dir/getSamps.c.s

# Object files for target rtlSamps
rtlSamps_OBJECTS = \
"CMakeFiles/rtlSamps.dir/rtlSamps.c.o" \
"CMakeFiles/rtlSamps.dir/getSamps.c.o"

# External object files for target rtlSamps
rtlSamps_EXTERNAL_OBJECTS =

rtlSamps: CMakeFiles/rtlSamps.dir/rtlSamps.c.o
rtlSamps: CMakeFiles/rtlSamps.dir/getSamps.c.o
rtlSamps: CMakeFiles/rtlSamps.dir/build.make
rtlSamps: CMakeFiles/rtlSamps.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable rtlSamps"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtlSamps.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtlSamps.dir/build: rtlSamps

.PHONY : CMakeFiles/rtlSamps.dir/build

CMakeFiles/rtlSamps.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtlSamps.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtlSamps.dir/clean

CMakeFiles/rtlSamps.dir/depend:
	cd /home/jojo/cradio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jojo/cradio /home/jojo/cradio /home/jojo/cradio/build /home/jojo/cradio/build /home/jojo/cradio/build/CMakeFiles/rtlSamps.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtlSamps.dir/depend
