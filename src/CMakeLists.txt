# Find packages for all dependencies
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS program_options )

# Include all dependencies
include_directories( ./ ${Boost_INCLUDE_DIRS} )

# Turn main souce code into a library
add_library(src example.cpp )

# Target libraries for all dependencies
target_link_libraries( src ${Boost_LIBRARIES} )
