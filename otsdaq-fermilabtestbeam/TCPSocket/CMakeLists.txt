#cet_set_compiler_flags(
# EXTRA_FLAGS -Wno-missing-field-initializers
#)

find_package(XercesC REQUIRED)

include_directories(
  ${XercesC_INCLUDE_DIRS}
  ${MPI_INCLUDE_PATH}
)

cet_make(LIBRARY_NAME TCPSocket
         LIBRARIES
                XmlUtilities
                Macros
                ${Boost_SYSTEM_LIBRARY}
                ${XDAQ_XERCESC}
                ${XercesC_LIBRARIES}
                ${MF_MESSAGELOGGER}
                pthread
                MF_MessageLogger
)

install_headers()
install_source()

#include_directories(${MPI_INCLUDE_PATH})

#cet_make(LIBRARY_NAME TCPSocket
#		LIBRARIES
#		XmlUtilities
#		Macros
#	    ${Boost_SYSTEM_LIBRARY}
#		${XDAQ_XERCESC}
#		pthread
#		MF_MessageLogger
#)

#install_headers()
#install_source()
