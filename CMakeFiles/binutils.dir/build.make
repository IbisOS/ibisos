# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dashiel/Desktop/OffDriveProjects/IbisOS

# Utility rule file for binutils.

# Include any custom commands dependencies for this target.
include CMakeFiles/binutils.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/binutils.dir/progress.make

CMakeFiles/binutils:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dashiel/Desktop/OffDriveProjects/IbisOS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Builds binutils for cross compiler"
	mkdir -p /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler/build/binutils
	cd /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler/build/binutils && ../../binutils/configure --target=i686-elf --prefix=/home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler/output --with-sysroot --disable-nls --disable-werror
	cd /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler/build/binutils && make -j4
	cd /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler/build/binutils && make -j4 install

binutils: CMakeFiles/binutils
binutils: CMakeFiles/binutils.dir/build.make
.PHONY : binutils

# Rule to build all files generated by this target.
CMakeFiles/binutils.dir/build: binutils
.PHONY : CMakeFiles/binutils.dir/build

CMakeFiles/binutils.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/binutils.dir/cmake_clean.cmake
.PHONY : CMakeFiles/binutils.dir/clean

CMakeFiles/binutils.dir/depend:
	cd /home/dashiel/Desktop/OffDriveProjects/IbisOS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler /home/dashiel/Desktop/OffDriveProjects/IbisOS/compiler /home/dashiel/Desktop/OffDriveProjects/IbisOS /home/dashiel/Desktop/OffDriveProjects/IbisOS /home/dashiel/Desktop/OffDriveProjects/IbisOS/CMakeFiles/binutils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/binutils.dir/depend
