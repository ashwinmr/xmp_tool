# cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# project name
project( cpp_example )

# add subdirectories
add_subdirectory (src)
add_subdirectory (test)

# include
include_directories( ./src )

# add executable
add_executable( cpp_example ./src/main.cpp )

# use main source code as a library
target_link_libraries( cpp_example src )

# testing
enable_testing()
add_test(
  NAME test_example
  COMMAND test_example
)
