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
CMAKE_SOURCE_DIR = /home/demesta/Documents/syspro3/whoServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/demesta/Documents/syspro3/whoServer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/whoServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/whoServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/whoServer.dir/flags.make

CMakeFiles/whoServer.dir/main.cpp.o: CMakeFiles/whoServer.dir/flags.make
CMakeFiles/whoServer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/whoServer.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoServer.dir/main.cpp.o -c /home/demesta/Documents/syspro3/whoServer/main.cpp

CMakeFiles/whoServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoServer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoServer/main.cpp > CMakeFiles/whoServer.dir/main.cpp.i

CMakeFiles/whoServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoServer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoServer/main.cpp -o CMakeFiles/whoServer.dir/main.cpp.s

CMakeFiles/whoServer.dir/List.cpp.o: CMakeFiles/whoServer.dir/flags.make
CMakeFiles/whoServer.dir/List.cpp.o: ../List.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/whoServer.dir/List.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoServer.dir/List.cpp.o -c /home/demesta/Documents/syspro3/whoServer/List.cpp

CMakeFiles/whoServer.dir/List.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoServer.dir/List.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoServer/List.cpp > CMakeFiles/whoServer.dir/List.cpp.i

CMakeFiles/whoServer.dir/List.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoServer.dir/List.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoServer/List.cpp -o CMakeFiles/whoServer.dir/List.cpp.s

CMakeFiles/whoServer.dir/Hash.cpp.o: CMakeFiles/whoServer.dir/flags.make
CMakeFiles/whoServer.dir/Hash.cpp.o: ../Hash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/whoServer.dir/Hash.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoServer.dir/Hash.cpp.o -c /home/demesta/Documents/syspro3/whoServer/Hash.cpp

CMakeFiles/whoServer.dir/Hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoServer.dir/Hash.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoServer/Hash.cpp > CMakeFiles/whoServer.dir/Hash.cpp.i

CMakeFiles/whoServer.dir/Hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoServer.dir/Hash.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoServer/Hash.cpp -o CMakeFiles/whoServer.dir/Hash.cpp.s

CMakeFiles/whoServer.dir/utils.cpp.o: CMakeFiles/whoServer.dir/flags.make
CMakeFiles/whoServer.dir/utils.cpp.o: ../utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/whoServer.dir/utils.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/whoServer.dir/utils.cpp.o -c /home/demesta/Documents/syspro3/whoServer/utils.cpp

CMakeFiles/whoServer.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/whoServer.dir/utils.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/demesta/Documents/syspro3/whoServer/utils.cpp > CMakeFiles/whoServer.dir/utils.cpp.i

CMakeFiles/whoServer.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/whoServer.dir/utils.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/demesta/Documents/syspro3/whoServer/utils.cpp -o CMakeFiles/whoServer.dir/utils.cpp.s

# Object files for target whoServer
whoServer_OBJECTS = \
"CMakeFiles/whoServer.dir/main.cpp.o" \
"CMakeFiles/whoServer.dir/List.cpp.o" \
"CMakeFiles/whoServer.dir/Hash.cpp.o" \
"CMakeFiles/whoServer.dir/utils.cpp.o"

# External object files for target whoServer
whoServer_EXTERNAL_OBJECTS =

whoServer: CMakeFiles/whoServer.dir/main.cpp.o
whoServer: CMakeFiles/whoServer.dir/List.cpp.o
whoServer: CMakeFiles/whoServer.dir/Hash.cpp.o
whoServer: CMakeFiles/whoServer.dir/utils.cpp.o
whoServer: CMakeFiles/whoServer.dir/build.make
whoServer: CMakeFiles/whoServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable whoServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/whoServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/whoServer.dir/build: whoServer

.PHONY : CMakeFiles/whoServer.dir/build

CMakeFiles/whoServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/whoServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/whoServer.dir/clean

CMakeFiles/whoServer.dir/depend:
	cd /home/demesta/Documents/syspro3/whoServer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/demesta/Documents/syspro3/whoServer /home/demesta/Documents/syspro3/whoServer /home/demesta/Documents/syspro3/whoServer/cmake-build-debug /home/demesta/Documents/syspro3/whoServer/cmake-build-debug /home/demesta/Documents/syspro3/whoServer/cmake-build-debug/CMakeFiles/whoServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/whoServer.dir/depend

