include_directories(${CMAKE_BINARY_DIR}/artdaq)

find_package(ROOT REQUIRED COMPONENTS Net)
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

link_directories(${Protobuf_LIBRARIES})

find_package(XercesC REQUIRED)

include_directories(
  ${XercesC_INCLUDE_DIRS}
  ${MPI_INCLUDE_PATH}
  )

cet_set_compiler_flags(
  EXTRA_FLAGS -D__OTSDAQ__
)


basic_plugin(FSSRInterface "interface" 
  Configurable
  DAQHardware
  DAQFirmwareHardware
  WorkLoopManager
  DetectorConfiguration
  ConfigurationInterface
  TableCore
  FECore
  NetworkUtilities
  Macros
  artdaq_DAQdata
  MF_MessageLogger
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  pthread
  )
  
# basic_plugin(FEOtsUDPFSSRInterface "interface"
#  DAQHardware
#  FECore
#  WorkLoopManager
#  DetectorConfiguration
#  ConfigurationInterface
#  TableCore
#  NetworkUtilities
#  ${CETLIB}
#  )
  
  basic_plugin(FTBFWireChamberInterface "interface"
  Configurable
  DAQHardware
  FECore
  WorkLoopManager
#  MessageFacility
  NetworkUtilities
  ConfigurationInterface
  TableCore
  TCPSocket
  Macros
  artdaq_DAQdata
  ${CETLIB}
  ${CETLIB_EXCEPT}
  ${Boost_SYSTEM_LIBRARY}
	${MF_MESSAGELOGGER}
  )
  
  basic_plugin(FECAMACInterface "interface"
  Configurable
  FECore
  WorkLoopManager
  ConfigurationInterface
  TableCore
  NetworkUtilities
  Macros
  artdaq_DAQdata
  xx_usb
  usb
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  MF_MessageLogger
  pthread
  )
  
  basic_plugin(FECAMACTCPInterface "interface"
  Configurable
  FECore
  WorkLoopManager
  ConfigurationInterface
  TableCore
  NetworkUtilities
  Macros
  artdaq_DAQdata
  xx_usb
  usb
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  MF_MessageLogger
  pthread
  )

 basic_plugin(FEPurdueNetworkMasterInterface "interface"
 Configurable
  DAQHardware
  DAQFirmwareHardware
  WorkLoopManager
  FECore
  DetectorConfiguration
  ConfigurationInterface
  TableCore
  NetworkUtilities
  Macros
  artdaq_DAQdata
  ${MF_MESSAGELOGGER}
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  pthread
  )

basic_plugin(FEPixelTelescopeInterface "interface"
  Configurable
  FECore
  DetectorConfiguration
  WorkLoopManager
  NetworkUtilities
  ConfigurationInterface
  TableCore
  Macros
  artdaq_DAQdata
  ${CETLIB}
	${MF_MESSAGELOGGER}
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  pthread
  )

basic_plugin(FECommanderInterface "interface"
  Configurable
  FECore
  DetectorConfiguration
  WorkLoopManager
  NetworkUtilities
  ConfigurationInterface
  Macros
  artdaq_DAQdata
  TableCore
  ${CETLIB}
  ${MF_MESSAGELOGGER}
  ${XDAQ_XERCES-C}
  ${Boost_SYSTEM_LIBRARY}
  pthread
  )
