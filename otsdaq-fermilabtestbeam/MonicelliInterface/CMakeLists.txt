find_package(ROOT REQUIRED COMPONENTS Core Hist Graf RIO Tree)
include_directories(${ROOT_INCLUDE_DIRS})
find_package(XercesC REQUIRED)
include_directories(
  ${XercesC_INCLUDE_DIRS}
  ${MPI_INCLUDE_PATH}
)

include(CetRootCint)
cet_rootcint(Monicelli)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
cet_make_library(LIBRARY_NAME MonicelliInterface
                SOURCE ${CMAKE_CURRENT_BINARY_DIR}/MonicelliCint.cc
                       Detector.cxx
                       Event.cxx
                       EventHeader.cxx
                       Geometry.cxx
                       MonicelliEventAnalyzer.cpp
                       MonicelliFileReader.cpp
                       MonicelliGeometryConverter.cpp
                       ROC.cxx
                LIBRARIES
                       ${ROOT_BASIC_LIB_LIST}
                       ${ROOT_LIBRARIES}
                       ${MF_MESSAGELOGGER}
                       ${XDAQ_XERCESC}
                       ${XercesC_LIBRARIES}
                       MF_MessageLogger
                       otsdaq::Macros
                )

install_headers()
install_source()


#include(CetRootCint)
#cet_rootcint(Monicelli)
#include_directories(${CMAKE_CURRENT_BINARY_DIR})
#cet_make_library(LIBRARY_NAME MonicelliInterface
#	        SOURCE ${CMAKE_CURRENT_BINARY_DIR}/MonicelliCint.cc
 #                      Detector.cxx
#		       Event.cxx
#		       EventHeader.cxx
#		       Geometry.cxx
#		       MonicelliEventAnalyzer.cpp
#		       MonicelliFileReader.cpp
#		       MonicelliGeometryConverter.cpp
#		       ROC.cxx
#		LIBRARIES
#		       ${ROOT_BASIC_LIB_LIST}
#		       MF_MessageLogger       
#                )
#install_headers()
#install_source()
