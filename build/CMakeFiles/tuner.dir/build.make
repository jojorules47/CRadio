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
include CMakeFiles/tuner.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tuner.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tuner.dir/flags.make

CMakeFiles/tuner.dir/tuner.c.o: CMakeFiles/tuner.dir/flags.make
CMakeFiles/tuner.dir/tuner.c.o: ../tuner.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tuner.dir/tuner.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tuner.dir/tuner.c.o   -c /home/jojo/cradio/tuner.c

CMakeFiles/tuner.dir/tuner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tuner.dir/tuner.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jojo/cradio/tuner.c > CMakeFiles/tuner.dir/tuner.c.i

CMakeFiles/tuner.dir/tuner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tuner.dir/tuner.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jojo/cradio/tuner.c -o CMakeFiles/tuner.dir/tuner.c.s

CMakeFiles/tuner.dir/kiss_fft.c.o: CMakeFiles/tuner.dir/flags.make
CMakeFiles/tuner.dir/kiss_fft.c.o: ../kiss_fft.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/tuner.dir/kiss_fft.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tuner.dir/kiss_fft.c.o   -c /home/jojo/cradio/kiss_fft.c

CMakeFiles/tuner.dir/kiss_fft.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tuner.dir/kiss_fft.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jojo/cradio/kiss_fft.c > CMakeFiles/tuner.dir/kiss_fft.c.i

CMakeFiles/tuner.dir/kiss_fft.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tuner.dir/kiss_fft.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jojo/cradio/kiss_fft.c -o CMakeFiles/tuner.dir/kiss_fft.c.s

CMakeFiles/tuner.dir/getSamps.c.o: CMakeFiles/tuner.dir/flags.make
CMakeFiles/tuner.dir/getSamps.c.o: ../getSamps.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/tuner.dir/getSamps.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tuner.dir/getSamps.c.o   -c /home/jojo/cradio/getSamps.c

CMakeFiles/tuner.dir/getSamps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tuner.dir/getSamps.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jojo/cradio/getSamps.c > CMakeFiles/tuner.dir/getSamps.c.i

CMakeFiles/tuner.dir/getSamps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tuner.dir/getSamps.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jojo/cradio/getSamps.c -o CMakeFiles/tuner.dir/getSamps.c.s

# Object files for target tuner
tuner_OBJECTS = \
"CMakeFiles/tuner.dir/tuner.c.o" \
"CMakeFiles/tuner.dir/kiss_fft.c.o" \
"CMakeFiles/tuner.dir/getSamps.c.o"

# External object files for target tuner
tuner_EXTERNAL_OBJECTS =

tuner: CMakeFiles/tuner.dir/tuner.c.o
tuner: CMakeFiles/tuner.dir/kiss_fft.c.o
tuner: CMakeFiles/tuner.dir/getSamps.c.o
tuner: CMakeFiles/tuner.dir/build.make
tuner: CMakeFiles/tuner.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jojo/cradio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable tuner"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tuner.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tuner.dir/build: tuner

.PHONY : CMakeFiles/tuner.dir/build

CMakeFiles/tuner.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tuner.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tuner.dir/clean

CMakeFiles/tuner.dir/depend:
	cd /home/jojo/cradio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jojo/cradio /home/jojo/cradio /home/jojo/cradio/build /home/jojo/cradio/build /home/jojo/cradio/build/CMakeFiles/tuner.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tuner.dir/depend

