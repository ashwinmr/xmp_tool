# cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# project name
project( test_example )

# Find packages
find_package( GTest )

# include
include_directories( ${GTEST_INCLUDE_DIRS} )

# target
add_executable( test_example ./test_example.cpp )

# external libs
target_link_libraries( test_example ${GTEST_BOTH_LIBRARIES} )

# Add tests
add_test(
  NAME test_example
  COMMAND test_example
)
