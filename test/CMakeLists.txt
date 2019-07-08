# Find packages
find_package( GTest )

# include
include_directories( ../src ${GTEST_INCLUDE_DIRS} )

# target
add_executable( test_example ./test_example.cpp )

# target external libs
target_link_libraries( test_example src ${GTEST_BOTH_LIBRARIES} pthread )
