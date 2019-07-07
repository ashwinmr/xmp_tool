# Find packages
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS program_options )

# include
include_directories( ${Boost_INCLUDE_DIRS} )

# Turn main souce code into a library
add_library(src example.cpp example.hpp)
