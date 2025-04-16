include_directories(${MPI_INCLUDE_PATH})

#cet_set_compiler_flags(
# EXTRA_FLAGS -Wno-missing-field-initializers
#)

cet_make(LIBRARY_NAME TCPSocket
		LIBRARIES
		XmlUtilities
		Macros
	    ${Boost_SYSTEM_LIBRARY}
		${XDAQ_XERCESC}
		pthread
		MF_MessageLogger
)

install_headers()
install_source()
