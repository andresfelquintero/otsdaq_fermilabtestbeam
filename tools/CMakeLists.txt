
# Nominally a test exec (no MPI) but we want this to be available to other packages.

# tools/CMakeLists.txt

# Always include the cetmodules build macros
include(CetMake)
include(ArtMake)

# Define the executable using art_make_exec
art_make_exec(
  NAME EventDataReceiver
  SOURCE EventDataReceiver.cc
  LIBRARIES
    artdaq_DAQdata
    artdaq-core_Data
    artdaq-core_Utilities
    cetlib
    cetlib_except
    fhiclcpp
    boost_system
    boost_program_options
    messagefacility::MF_MessageLogger
)

install(TARGETS EventDataReceiver DESTINATION ${CMAKE_INSTALL_BINDIR})

install_source()

# find_package(artdaq-core REQUIRED)

# art_make_exec(NAME EventDataReceiver
#     SOURCE
#     EventDataReceiver.cc
#     LIBRARIES
#     artdaq_DAQdata
#     artdaq_core::artdaq-core_Data
# 	artdaq_core::artdaq-core_Utilities
#     ${FHICLCPP}
#     ${CETLIB}
# 	${CETLIB_EXCEPT}
#     ${Boost_PROGRAM_OPTIONS_LIBRARY}
# 	${Boost_SYSTEM_LIBRARY}
# 	${MF_MESSAGELOGGER}
#     )


