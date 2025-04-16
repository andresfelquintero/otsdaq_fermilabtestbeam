art_make(BASENAME_ONLY
 LIBRARY_NAME otsdaq-fermilabtestbeam_Generators
  NO_PLUGINS
  EXCLUDE STIBReceiver_generator.cc STIBEventReceiver_generator.cc WireChamberEventReceiver_generator.cc WireChamberReceiver_generator.cc CAMACReceiver_generator.cc
 LIB_LIBRARIES
 artdaq-core_Data
 artdaq_DAQdata
 artdaq_Generators
 ${FHICLCPP}
 ${MF_MESSAGELOGGER}
 ${CETLIB}
 ${CETLIB_EXCEPT}
 ${Boost_SYSTEM_LIBRARY}
 ${Boost_THREAD_LIBRARY}
  )


simple_plugin(STIBReceiver "generator"
  artdaq-ots_Overlays
  otsdaq-fermilabtestbeam_Overlays
  artdaq-ots_Generators_UDPReceiver_generator
  artdaq_Generators
  artdaq_DAQdata
  artdaq_DAQrate
  artdaq-core_Utilities
  artdaq-core_Data
  ${MF_MESSAGELOGGER}
  ${ART_UTILITIES}
  ${CANVAS}
  ${FHICLCPP}
  ${CETLIB}
 ${CETLIB_EXCEPT}
 ${Boost_SYSTEM_LIBRARY}
pthread
  )

simple_plugin(CAMACReceiver "generator"
  DataManager
  WorkLoopManager
  artdaq-ots_Overlays
  otsdaq-fermilabtestbeam_Overlays
#  artdaq-ots_Generators_UDPReceiver_generator
  artdaq_Generators
  artdaq_DAQdata
  artdaq-core_Utilities
  artdaq-core_Data
  ${ART_UTILITIES}
  ${MF_MESSAGELOGGER}
  ${FHICLCPP}
  ${CETLIB}
  ${CETLIB_EXCEPT}
 ${Boost_SYSTEM_LIBRARY}
  )

simple_plugin(WireChamberReceiver "generator"
  DataManager
  WorkLoopManager
  artdaq-ots_Overlays
  otsdaq-fermilabtestbeam_Overlays
#  artdaq-ots_Generators_UDPReceiver_generator
  artdaq_Generators
  artdaq_DAQdata
  artdaq-core_Utilities
  artdaq-core_Data
  ${ART_UTILITIES}
  ${MF_MESSAGELOGGER}
  ${FHICLCPP}
  ${CETLIB}
  ${CETLIB_EXCEPT}
 ${Boost_SYSTEM_LIBRARY}
  )

simple_plugin(WireChamberEventReceiver "generator"
  DataManager
  WorkLoopManager
  artdaq-ots_Overlays
  otsdaq-fermilabtestbeam_Overlays
#  artdaq-ots_Generators_UDPReceiver_generator
  artdaq_Generators
  artdaq_DAQdata
  artdaq-core_Utilities
  artdaq-core_Data
  ${ART_UTILITIES}
  ${MF_MESSAGELOGGER}
  ${FHICLCPP}
  ${CETLIB}
  ${CETLIB_EXCEPT}
 ${Boost_SYSTEM_LIBRARY}
  )
