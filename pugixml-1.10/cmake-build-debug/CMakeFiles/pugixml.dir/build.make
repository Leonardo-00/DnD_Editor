# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.4\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\leona\Desktop\pugixml-1.10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/pugixml.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pugixml.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pugixml.dir/flags.make

CMakeFiles/pugixml.dir/src/pugixml.cpp.obj: CMakeFiles/pugixml.dir/flags.make
CMakeFiles/pugixml.dir/src/pugixml.cpp.obj: CMakeFiles/pugixml.dir/includes_CXX.rsp
CMakeFiles/pugixml.dir/src/pugixml.cpp.obj: ../src/pugixml.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pugixml.dir/src/pugixml.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\pugixml.dir\src\pugixml.cpp.obj -c C:\Users\leona\Desktop\pugixml-1.10\src\pugixml.cpp

CMakeFiles/pugixml.dir/src/pugixml.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/pugixml.dir/src/pugixml.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\leona\Desktop\pugixml-1.10\src\pugixml.cpp > CMakeFiles\pugixml.dir\src\pugixml.cpp.i

CMakeFiles/pugixml.dir/src/pugixml.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/pugixml.dir/src/pugixml.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\leona\Desktop\pugixml-1.10\src\pugixml.cpp -o CMakeFiles\pugixml.dir\src\pugixml.cpp.s

# Object files for target pugixml
pugixml_OBJECTS = \
"CMakeFiles/pugixml.dir/src/pugixml.cpp.obj"

# External object files for target pugixml
pugixml_EXTERNAL_OBJECTS =

libpugixml.a: CMakeFiles/pugixml.dir/src/pugixml.cpp.obj
libpugixml.a: CMakeFiles/pugixml.dir/build.make
libpugixml.a: CMakeFiles/pugixml.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libpugixml.a"
	$(CMAKE_COMMAND) -P CMakeFiles\pugixml.dir\cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\pugixml.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pugixml.dir/build: libpugixml.a

.PHONY : CMakeFiles/pugixml.dir/build

CMakeFiles/pugixml.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\pugixml.dir\cmake_clean.cmake
.PHONY : CMakeFiles/pugixml.dir/clean

CMakeFiles/pugixml.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\leona\Desktop\pugixml-1.10 C:\Users\leona\Desktop\pugixml-1.10 C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug C:\Users\leona\Desktop\pugixml-1.10\cmake-build-debug\CMakeFiles\pugixml.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pugixml.dir/depend
