# Cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# Project name
project( xmp_tools )

# Add subdirectories
add_subdirectory (src)
add_subdirectory (test)

# Include all dependencies
<<<<<<< HEAD
include_directories( ./include ${Boost_INCLUDE_DIRS} )
=======
include_directories( ./include )
>>>>>>> upstream/master

# Add executable
add_executable( xmp_tools ./src/main.cpp )

# Use main source code as a library and target it
target_link_libraries( xmp_tools src )

# Install
install(TARGETS xmp_tools DESTINATION bin)

# Testing
enable_testing()
add_test(
  NAME test_xmp_tools
  COMMAND test_xmp_tools
)
