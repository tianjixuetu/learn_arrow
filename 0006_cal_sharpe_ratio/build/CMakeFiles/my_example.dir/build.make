# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build

# Include any dependencies generated for this target.
include CMakeFiles/my_example.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/my_example.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/my_example.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/my_example.dir/flags.make

CMakeFiles/my_example.dir/my_example.cc.o: CMakeFiles/my_example.dir/flags.make
CMakeFiles/my_example.dir/my_example.cc.o: /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/my_example.cc
CMakeFiles/my_example.dir/my_example.cc.o: CMakeFiles/my_example.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/my_example.dir/my_example.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/my_example.dir/my_example.cc.o -MF CMakeFiles/my_example.dir/my_example.cc.o.d -o CMakeFiles/my_example.dir/my_example.cc.o -c /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/my_example.cc

CMakeFiles/my_example.dir/my_example.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/my_example.dir/my_example.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/my_example.cc > CMakeFiles/my_example.dir/my_example.cc.i

CMakeFiles/my_example.dir/my_example.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/my_example.dir/my_example.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/my_example.cc -o CMakeFiles/my_example.dir/my_example.cc.s

# Object files for target my_example
my_example_OBJECTS = \
"CMakeFiles/my_example.dir/my_example.cc.o"

# External object files for target my_example
my_example_EXTERNAL_OBJECTS =

my_example: CMakeFiles/my_example.dir/my_example.cc.o
my_example: CMakeFiles/my_example.dir/build.make
my_example: /home/yun/anaconda3/lib/libarrow_dataset.so.1100.0.0
my_example: /home/yun/anaconda3/lib/libparquet.so.1100.0.0
my_example: /home/yun/anaconda3/lib/libarrow.so.1100.0.0
my_example: CMakeFiles/my_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable my_example"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/my_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/my_example.dir/build: my_example
.PHONY : CMakeFiles/my_example.dir/build

CMakeFiles/my_example.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/my_example.dir/cmake_clean.cmake
.PHONY : CMakeFiles/my_example.dir/clean

CMakeFiles/my_example.dir/depend:
	cd /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build /home/yun/learn_series/learn_arrow/0006_cal_sharpe_ratio/build/CMakeFiles/my_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/my_example.dir/depend

