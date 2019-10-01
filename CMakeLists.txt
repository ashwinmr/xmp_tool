# Cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# Project name
project( xmp_tools )

# Add subdirectories
add_subdirectory (src)
add_subdirectory (test)

# Find packages for all dependencies used by main
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS program_options filesystem)

# Include all dependencies
include_directories( ./include ${Boost_INCLUDE_DIRS})

# Add executable
add_executable( xmp_tools ./src/main.cpp )

# Use main source code as a library and target it
target_link_libraries( xmp_tools src ${Boost_LIBRARIES})

# Install
install(TARGETS xmp_tools DESTINATION bin)

# Testing
enable_testing()
add_test(
  NAME test_xmp_tools
  COMMAND test_xmp_tools
)
