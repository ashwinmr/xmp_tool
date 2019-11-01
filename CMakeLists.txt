# Cmake version to be used
cmake_minimum_required( VERSION 3.1 )

# Set minimum cpp version
set (CMAKE_CXX_STANDARD 11)

# Project name
project( xmp_tool )

# Add subdirectories
add_subdirectory (src)
add_subdirectory (test)

# Find packages for all dependencies used by main
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS filesystem)

# Include all dependencies
include_directories( ./include ./src/libs/sqlite3/include ${Boost_INCLUDE_DIRS})

# Add executable
add_executable( xmp_tool ./src/main.cpp )

# Use main source code as a library and target it
target_link_libraries( xmp_tool src ${Boost_LIBRARIES})

# Install
install(TARGETS xmp_tool DESTINATION bin) # This is relative to cmake install prefix or -DCMAKE_INSTALL_PREFIX

# Testing
enable_testing()
add_test(
  NAME test_xmp_tool
  COMMAND test_xmp_tool
)
