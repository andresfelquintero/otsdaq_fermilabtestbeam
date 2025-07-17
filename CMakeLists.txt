# ======================================================================
#  otsdaq-fermilabtestbeam main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/otsdaq-fermilabtestbeam/ups/setup_for_development
#  buildtool
# ======================================================================


message(STATUS "This is MY CMakeLists.txt in otsdaq_fermilabtestbeam! #2")
# Enforce C++20 for concept-based code
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(IGNORE_ABSOLUTE_TRANSITIVE_DEPENDENCIES TRUE)
# use cmake 3.17 or later
cmake_minimum_required (VERSION 3.17)

find_package(cetmodules)
set(CET_CETBUILDTOOLS_COMPAT TRUE)
include(Compatibility)
set(CET_WARN_DEPRECATED)

project(otsdaq_fermilabtestbeam VERSION 2.06.03)

find_package(messagefacility REQUIRED)
#set(CMAKE_MODULE_PATH
#        $ENV{ART_DIR}/Modules
#        $ENV{CETBUILDTOOLS_DIR}/Modules
#        $ENV{XDAQ_DIR}/Modules
#        $ENV{ARTDAQ_UTILITIES_DIR}/Modules
#        ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH is ${CMAKE_MODULE_PATH}")

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-ignored-qualifiers -Wno-unused-variable
)

#string(TOUPPER ${CMAKE_BUILD_TYPE} BTYPE_UC )
#if( ${BTYPE_UC} MATCHES "DEBUG" )
#  cet_add_compiler_flags(-fsanitize=address)
#endif()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_package( TRACE v3_17_00 )
find_package( otsdaq v1_00_00 )
find_package( otsdaq_utilities v2_00_00 )
#find_package( epics-base )
#find_package( cetbuildtools )
find_package( artdaq v3_09_00 )
find_package( artdaq_core v3_06_00 )
find_package( artdaq_utilities v1_00_00 )
find_package( art v1_06_00 )
#find_package( fhicl-cpp )
find_package( messagefacility v1_10_00 )
find_package( cetlib v1_03_00 )
find_package( otsdaq_components v1_00_00 )
find_package( xdaq v16_7_0_1 )
#find_package( root v5_34_00 )
find_package( ROOT REQUIRED COMPONENTS Core Imt Hist Gpad Tree)
find_package( Boost QUIET COMPONENTS date_time program_options serialization thread iostreams REQUIRED EXPORT)


include_directories($ENV{XDAQ_INC}/linux $ENV{XDAQ_INC})
link_directories($ENV{XDAQ_LIB})
 set(XDAQ_BASIC_LIB_LIST
   xcept
   xoap
   xdaq
   toolbox
   xerces-c
   cgicc
   Threads::Threads
 )


#include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)
include(CetTest)

# Code
add_subdirectory(otsdaq-fermilabtestbeam)
add_subdirectory(tools)

# Documentation directory
#add_subdirectory(doc)

# Test Programs
#add_subdirectory(test)

# packaging utility
include(UseCPack)

cet_cmake_config()
