# cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# project name
project( cpp_example )

# add libraries
add_subdirectory (src)
add_subdirectory (test)

# testing
enable_testing()
add_test(
  NAME test_example
  COMMAND test_example
)
