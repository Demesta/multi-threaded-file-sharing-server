# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /home/demesta/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.7223.86/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/demesta/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/201.7223.86/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/demesta/Documents/syspro3/whoClient

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/demesta/Documents/syspro3/whoClient/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/whoClient.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/whoClient.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/whoClient.dir/flags.make

CMakeFiles/whoClient.dir/main.cpp.o: CMakeFiles/whoClient.dir/flags.make
CMakeFiles/whoClient.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoClient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/whoClient.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoClient.dir/main.cpp.o -c /home/demesta/Documents/syspro3/whoClient/main.cpp

CMakeFiles/whoClient.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoClient.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoClient/main.cpp > CMakeFiles/whoClient.dir/main.cpp.i

CMakeFiles/whoClient.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoClient.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoClient/main.cpp -o CMakeFiles/whoClient.dir/main.cpp.s

CMakeFiles/whoClient.dir/sockets.cpp.o: CMakeFiles/whoClient.dir/flags.make
CMakeFiles/whoClient.dir/sockets.cpp.o: ../sockets.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoClient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/whoClient.dir/sockets.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoClient.dir/sockets.cpp.o -c /home/demesta/Documents/syspro3/whoClient/sockets.cpp

CMakeFiles/whoClient.dir/sockets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoClient.dir/sockets.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoClient/sockets.cpp > CMakeFiles/whoClient.dir/sockets.cpp.i

CMakeFiles/whoClient.dir/sockets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoClient.dir/sockets.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoClient/sockets.cpp -o CMakeFiles/whoClient.dir/sockets.cpp.s

CMakeFiles/whoClient.dir/logging.cpp.o: CMakeFiles/whoClient.dir/flags.make
CMakeFiles/whoClient.dir/logging.cpp.o: ../logging.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoClient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/whoClient.dir/logging.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoClient.dir/logging.cpp.o -c /home/demesta/Documents/syspro3/whoClient/logging.cpp

CMakeFiles/whoClient.dir/logging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoClient.dir/logging.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoClient/logging.cpp > CMakeFiles/whoClient.dir/logging.cpp.i

CMakeFiles/whoClient.dir/logging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoClient.dir/logging.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoClient/logging.cpp -o CMakeFiles/whoClient.dir/logging.cpp.s

# Object files for target whoClient
whoClient_OBJECTS = \
"CMakeFiles/whoClient.dir/main.cpp.o" \
"CMakeFiles/whoClient.dir/sockets.cpp.o" \
"CMakeFiles/whoClient.dir/logging.cpp.o"

# External object files for target whoClient
whoClient_EXTERNAL_OBJECTS =

whoClient: CMakeFiles/whoClient.dir/main.cpp.o
whoClient: CMakeFiles/whoClient.dir/sockets.cpp.o
whoClient: CMakeFiles/whoClient.dir/logging.cpp.o
whoClient: CMakeFiles/whoClient.dir/build.make
whoClient: CMakeFiles/whoClient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/demesta/Documents/syspro3/whoClient/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable whoClient"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/whoClient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/whoClient.dir/build: whoClient

.PHONY : CMakeFiles/whoClient.dir/build

CMakeFiles/whoClient.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/whoClient.dir/cmake_clean.cmake
.PHONY : CMakeFiles/whoClient.dir/clean

CMakeFiles/whoClient.dir/depend:
	cd /home/demesta/Documents/syspro3/whoClient/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/demesta/Documents/syspro3/whoClient /home/demesta/Documents/syspro3/whoClient /home/demesta/Documents/syspro3/whoClient/cmake-build-debug /home/demesta/Documents/syspro3/whoClient/cmake-build-debug /home/demesta/Documents/syspro3/whoClient/cmake-build-debug/CMakeFiles/whoClient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/whoClient.dir/depend

