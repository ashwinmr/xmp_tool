# Find packages
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS program_options )

# include
include_directories( ${Boost_INCLUDE_DIRS} )

# create library for use in main
add_library(example example.cpp example.hpp)

# add executable
add_executable( cpp_example main.cpp )

# target external libs
target_link_libraries( cpp_example example ${Boost_LIBRARIES} )
