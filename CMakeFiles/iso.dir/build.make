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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dashiel/Desktop/Projects/Fun/IbisOS/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dashiel/Desktop/Projects/Fun/IbisOS

# Utility rule file for iso.

# Include the progress variables for this target.
include CMakeFiles/iso.dir/progress.make

CMakeFiles/iso:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/dashiel/Desktop/Projects/Fun/IbisOS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building IbisOS image file"
	grub-mkrescue -o output/i686-pc-none-elf-IbisOS.iso output/i686-pc-none-elf/isodir/boot/../

iso: CMakeFiles/iso
iso: CMakeFiles/iso.dir/build.make

.PHONY : iso

# Rule to build all files generated by this target.
CMakeFiles/iso.dir/build: iso

.PHONY : CMakeFiles/iso.dir/build

CMakeFiles/iso.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/iso.dir/cmake_clean.cmake
.PHONY : CMakeFiles/iso.dir/clean

CMakeFiles/iso.dir/depend:
	cd /home/dashiel/Desktop/Projects/Fun/IbisOS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dashiel/Desktop/Projects/Fun/IbisOS/src /home/dashiel/Desktop/Projects/Fun/IbisOS/src /home/dashiel/Desktop/Projects/Fun/IbisOS /home/dashiel/Desktop/Projects/Fun/IbisOS /home/dashiel/Desktop/Projects/Fun/IbisOS/CMakeFiles/iso.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/iso.dir/depend

