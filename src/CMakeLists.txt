# Find packages for all dependencies
set(Boost_USE_STATIC_LIBS ON)
find_package( Boost COMPONENTS program_options )

# Include all dependencies
include_directories(../include ../libs/XMP/include ${Boost_INCLUDE_DIRS} )

# Turn main souce code into a library
add_library(src xmp_tools.cpp )

# external libs that are pre compiled
add_library(XMPCoreStatic STATIC IMPORTED)
set_property(TARGET XMPCoreStatic PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/XMP/XMPCoreStatic.lib)
target_link_libraries( src XMPCoreStatic )
add_library(XMPFilesStatic STATIC IMPORTED)
set_property(TARGET XMPFilesStatic PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/XMP/XMPFilesStatic.lib)
target_link_libraries( src XMPFilesStatic )

# Target libraries for all dependencies
target_link_libraries( src ${Boost_LIBRARIES} )

# Add compiler definitions
add_compile_definitions(WIN_ENV)
