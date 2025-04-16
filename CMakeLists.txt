# ======================================================================
#  otsdaq-fermilabtestbeam main build file
#
#  cd .../path/to/build/directory
#  source .../path/to/otsdaq-fermilabtestbeam/ups/setup_for_development
#  buildtool
# ======================================================================


message(STATUS "This is MY CMakeLists.txt in otsdaq_fermilabtestbeam!")

# use cmake 3.17 or later
cmake_minimum_required (VERSION 3.17)

find_package(cetmodules)
set(CET_CETBUILDTOOLS_COMPAT TRUE)
include(Compatibility)
set(CET_WARN_DEPRECATED)

project(otsdaq_fermilabtestbeam VERSION 2.06.03)

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
find_package( root v5_34_00 )
find_package( Boost QUIET COMPONENTS date_time program_options serialization thread iostreams REQUIRED EXPORT)
find_package( Protobuf v3.19.4 REQUIRED)
#find_package (CACTUS REQUIRED)


#find_ups_product( epics v3_15_2 )
#find_ups_product( TRACE v3_17_00 )
#find_ups_product( cetbuildtools v3_03_00 )
#find_ups_product( artdaq v3_09_00 )
#find_ups_product( artdaq_core v3_06_00 )
#find_ups_product( artdaq_utilities v1_00_00 )
#find_ups_product( art v1_06_00 )
#find_ups_product( fhiclcpp v2_16_00 )
#find_ups_product( messagefacility v1_10_00 )
#find_ups_product( cetlib v1_03_00 )
#find_ups_product( otsdaq v1_00_00 )
#find_ups_product( otsdaq_utilities v2_00_00 )
#find_ups_product( otsdaq_components v1_00_00 )
#find_ups_product( xdaq v16_7_0_1)
#find_ups_root( v5_34_00 )
#find_ups_boost( v1_50_0 filesystem
#                        program_options
#                        unit_test_framework
# )

# XDAQ Extra setup
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

#find_ups_product( xmlrpc_c v1_25_18 )


#find_library( XMLRPC_SERVER_ABYSSPP NAMES xmlrpc_server_abyss++ PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_SERVER_ABYSS NAMES xmlrpc_server_abyss PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_SERVERPP NAMES xmlrpc_server++ PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_SERVER NAMES xmlrpc_server PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_ABYSS NAMES xmlrpc_abyss PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_CLIENTPP NAMES xmlrpc_client++ PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPCPP NAMES xmlrpc++ PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC NAMES xmlrpc PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_UTIL NAMES xmlrpc_util PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_UTILPP NAMES xmlrpc_util++ PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
# XMLRPC_UTILPP is new to v1_39_07:
#if(NOT XMLRPC_UTILPP)
#       set(XMLRPC_UTILPP "")
#endif()
#find_library( XMLRPC_XMLPARSE NAMES xmlrpc_xmlparse PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )
#find_library( XMLRPC_XMLTOK NAMES xmlrpc_xmltok PATHS $ENV{XMLRPC_C_LIBRARY} NO_DEFAULT_PATH )

#string(REGEX MATCHALL "[0-9][0-9]?[0-9]?" ART_VERSION_LIST "$ENV{ART_VERSION}")
#list(GET ART_VERSION_LIST 0 ART_MAJOR_VERSION)
#list(GET ART_VERSION_LIST 1 ART_MINOR_VERSION)
#list(GET ART_VERSION_LIST 2 ART_PATCH_VERSION)

# string(LENGTH ${ART_MINOR_VERSION} TMP_LEN)
# if (${TMP_LEN} GREATER 1)
#     string(SUBSTRING ${ART_MINOR_VERSION} 0 1 TMP_STRING)
#     if (${TMP_STRING} EQUAL "0")
#         string(SUBSTRING ${ART_MINOR_VERSION} 1 ${TMP_LEN} ART_MINOR_VERSION)
#     endif()
# endif()

# string(LENGTH ${ART_PATCH_VERSION} TMP_LEN)
# if (${TMP_LEN} GREATER 1)
#     string(SUBSTRING ${ART_PATCH_VERSION} 0 1 TMP_STRING)
#     if (${TMP_STRING} EQUAL "0")
#         string(SUBSTRING ${ART_PATCH_VERSION} 1 ${TMP_LEN} ART_PATCH_VERSION)
#     endif()
# endif()

# message("-DART_MAJOR_VERSION=${ART_MAJOR_VERSION} -DART_MINOR_VERSION=${ART_MINOR_VERSION} -DART_PATCH_VERSION=${ART_PATCH_VERSION}")
# add_definitions(-DART_MAJOR_VERSION=${ART_MAJOR_VERSION} -DART_MINOR_VERSION=${ART_MINOR_VERSION} -DART_PATCH_VERSION=${ART_PATCH_VERSION})
#message("-DART_HEX_VERSION=0x${ART_MAJOR_VERSION}${ART_MINOR_VERSION}${ART_PATCH_VERSION}")
#set(ART_HEX_VERSION 0x${ART_MAJOR_VERSION}${ART_MINOR_VERSION}${ART_PATCH_VERSION})
#add_definitions(-DART_HEX_VERSION=0x${ART_MAJOR_VERSION}${ART_MINOR_VERSION}${ART_PATCH_VERSION})

#string(REGEX MATCHALL "[0-9][0-9]?[0-9]?" MESSAGEFACILITY_VERSION_LIST "$ENV{MESSAGEFACILITY_VERSION}")
#list(GET MESSAGEFACILITY_VERSION_LIST 0 MESSAGEFACILITY_MAJOR_VERSION)
#list(GET MESSAGEFACILITY_VERSION_LIST 1 MESSAGEFACILITY_MINOR_VERSION)
#list(GET MESSAGEFACILITY_VERSION_LIST 2 MESSAGEFACILITY_PATCH_VERSION)

#message("-DMESSAGEFACILITY_HEX_VERSION=0x${MESSAGEFACILITY_MAJOR_VERSION}${MESSAGEFACILITY_MINOR_VERSION}${MESSAGEFACILITY_PATCH_VERSION}")
#add_definitions(-DMESSAGEFACILITY_HEX_VERSION=0x${MESSAGEFACILITY_MAJOR_VERSION}${MESSAGEFACILITY_MINOR_VERSION}${MESSAGEFACILITY_PATCH_VERSION})

# Needed if using old art with new cetbuildtools / CMake.
#check_ups_version(art "$ENV{ART_VERSION}" v1_11_00 PRODUCT_OLDER_VAR have_old_art)
#if (have_old_art)
#  cmake_policy(SET CMP0046 OLD)
#endif()
#unset(have_old_art)

#find_package(MPI QUIET)

# macros for art_dictionary and simple_plugin
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
