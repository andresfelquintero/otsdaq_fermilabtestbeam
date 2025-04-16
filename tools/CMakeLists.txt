
# Nominally a test exec (no MPI) but we want this to be available to other packages.


art_make_exec(NAME EventDataReceiver
    SOURCE
    EventDataReceiver.cc
    LIBRARIES
    artdaq_DAQdata
    artdaq_core::artdaq-core_Data
	artdaq_core::artdaq-core_Utilities
    ${FHICLCPP}
    ${CETLIB}
	${CETLIB_EXCEPT}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${MF_MESSAGELOGGER}
    )

