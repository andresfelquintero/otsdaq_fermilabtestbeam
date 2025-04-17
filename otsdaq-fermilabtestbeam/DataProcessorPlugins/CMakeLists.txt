include(otsdaq::dataProcessor)

include_directories(${CMAKE_BINARY_DIR}/artdaq)
#include_directories(${Protobuf_INCLUDE_DIRS})
#include_directories(${PH2_ACF_CMAKE_DIR})

find_package(ROOT REQUIRED COMPONENTS Net)
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})

# You can also print this info for debug:
message(STATUS "ROOT include: ${ROOT_INCLUDE_DIRS}")
message(STATUS "ROOT libs: ${ROOT_LIBRARIES}")

link_directories(${Protobuf_LIBRARIES})

cet_set_compiler_flags(
  EXTRA_FLAGS -D__OTSDAQ__
)

#MESSAGE(STATUS ${UHAL_UHAL_INCLUDE_PREFIX})
cet_build_plugin(FSSRDQMHistosConsumer otsdaq::dataProcessor
  LIBRARIES REG
  Configurable
  ${CETLIB}
  DataManager
  RootUtilities
  FermilabTestbeamDQMHistos
  WorkLoopManager
  MF_MessageLogger
  )

cet_build_plugin(FSSRRawDataSaverConsumer otsdaq::dataProcessor
  DataManager
  WorkLoopManager
  ${CETLIB}
  MF_MessageLogger
  )

cet_build_plugin(WireChamberDQMHistosConsumer otsdaq::dataProcessor
  Configurable
  DataManager
  RootUtilities
  FermilabTestbeamDQMHistos
  WorkLoopManager
  ${CETLIB}
 )

cet_build_plugin(WireChamberDataSaverConsumer otsdaq::dataProcessor
  DataManager
  WorkLoopManager
  ${CETLIB}
  ${MF_MESSAGELOGGER}
  )

cet_build_plugin(RootDQMHistosConsumer otsdaq::dataProcessor
  Configurable
  RootUtilities
  DataManager
  WorkLoopManager
  ConfigurationInterface
          Macros
          ${ROOT_BASIC_LIB_LIST}
          ${MF_MESSAGELOGGER}
  )

cet_build_plugin(UDPFSSRDataListenerProducer otsdaq::dataProcessor
        DataManager
        NetworkUtilities
        ${CETLIB}
  )

cet_build_plugin(UDPFSSRDataStreamerConsumer otsdaq::dataProcessor
        DataManager
        NetworkUtilities
        ${CETLIB}
  )


#cet_build_plugin(TCPDataReceiverProducerCMSTracker otsdaq::dataProcessor 
#  LIBRARIES REG
#)

install_headers()
install_source()
