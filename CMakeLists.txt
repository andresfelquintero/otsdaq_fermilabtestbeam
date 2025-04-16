# use cmake 3.17 or later
cmake_minimum_required (VERSION 3.17)

find_package(cetmodules)
set(CET_CETBUILDTOOLS_COMPAT TRUE)
include(Compatibility)
set(CET_WARN_DEPRECATED)

project(otsdaq_fermilabtestbeam VERSION 2.06.03)

set(CMAKE_MODULE_PATH
        $ENV{ART_DIR}/Modules
        $ENV{CETBUILDTOOLS_DIR}/Modules
        $ENV{XDAQ_DIR}/Modules
        $ENV{ARTDAQ_UTILITIES_DIR}/Modules
        ${CMAKE_MODULE_PATH})
message(STATUS "CMAKE_MODULE_PATH is ${CMAKE_MODULE_PATH}")

include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS VIGILANT
 WERROR
 NO_UNDEFINED
 EXTRA_FLAGS -pedantic -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-maybe-uninitialized -Wno-ignored-qualifiers -Wno-unused-variable
)

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
#find_ups_product( epics v3_15_2 )
find_ups_product( cetbuildtools v3_03_00 )
find_ups_product( artdaq v3_09_00 )
find_ups_product( artdaq_core v3_06_00 )
find_ups_product( artdaq_utilities v1_00_00 )
find_ups_product( art v1_06_00 )
find_ups_product( fhiclcpp v2_16_00 )
find_ups_product( messagefacility v1_10_00 )
find_ups_product( cetlib v1_03_00 )
find_ups_product( otsdaq v1_00_00 )
find_ups_product( otsdaq_utilities v2_00_00 )
find_ups_product( otsdaq_components v1_00_00 )
find_ups_product( xdaq v16_7_0_1)
find_ups_product( TRACE v3_17_00 )
find_ups_root( v5_34_00 )
find_ups_boost( v1_50_0 filesystem
                        program_options
                        unit_test_framework
 )

# XDAQ Extra setup
include_directories($ENV{XDAQ_INC}/linux)
link_directories($ENV{XDAQ_LIB})


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
