art_make( BASENAME_ONLY
 LIBRARY_NAME otsdaq-fermilabtestbeam_ArtModules
 SUBDIRS detail
  NO_PLUGINS
  EXCLUDE anatka.C
  DICT_DICTIONARIES
  ${ART_PERSISTENCY_PROVENANCE}
  ${CANVAS_PERSISTENCY_PROVENANCE}
 LIB_LIBRARIES
  canvas
  otsdaq-fermilabtestbeam_Overlays
  #otsdaq-utilities_ECLWriter
  artdaq-core_ArtModules
  artdaq-ots_ArtModules
  artdaq_Generators
  artdaq_DAQdata
  artdaq_core::artdaq-core_Utilities
  artdaq_core::artdaq-core_Data
  art_Framework_Core
  art_Framework_IO_Sources
  art_Framework_Principal
  art_Framework_Services_Registry
  art_Persistency_Provenance
  MF_MessageLogger
  ${ROOT_CINTEX}
  ${ROOT_RIO}
  ${ROOT_CORE}
  ${ROOT_NET}
  ${ROOT_REFLEX}
  cetlib
  cetlib_except
)

simple_plugin(EventDataReceiverProducer "module" otsdaq-fermilabtestbeam_Overlays artdaq_DAQdata artdaq-core_Data)

simple_plugin(WireChamberDQM "module"
	otsdaq-fermilabtestbeam_Overlays
        ECLWriterCore
  NetworkUtilities
	artdaq-core_Data
	${CETLIB_EXCEPT}
	${ROOT_CORE}
	${ROOT_GPAD}
	${ROOT_HIST}
	${ROOT_GUI}
	${ROOT_RIO}
  ${ROOT_NET}
#	MessageFacility
	${MF_MESSAGELOGGER}
	${MF_UTILITIES}
	${XMLRPC_SERVER_ABYSSPP}
	${XMLRPC_SERVER_ABYSS}
	${XMLRPC_SERVERPP}
	${XMLRPC_CLIENT}
	${XMLRPC_CLIENTPP}
	${XMLRPC_SERVER}
	${XMLRPC_ABYSS}
	${XMLRPCPP}
	${XMLRPC}
	${XMLRPC_UTIL}
	${XMLRPC_UTILPP}
	${XMLRPC_XMLPARSE}
	${XMLRPC_XMLTOK}
	${XDAQ_XERCES-C}
	curl
	pthread
)
simple_plugin(WireChamberDQMECL "module"
	otsdaq-fermilabtestbeam_Overlays
        ECLWriterCore
  NetworkUtilities
	artdaq-core_Data
	${CETLIB_EXCEPT}
	${ROOT_CORE}
	${ROOT_GPAD}
	${ROOT_HIST}
	${ROOT_GUI}
	${ROOT_RIO}
  ${ROOT_NET}
#	MessageFacility
	${MF_MESSAGELOGGER}
	${MF_UTILITIES}
	${XDAQ_XERCES-C}
	pthread
)


simple_plugin(WireChamberPythonOutput "module"
        otsdaq-fermilabtestbeam_Overlays
        artdaq-core_Data
        ${CETLIB_EXCEPT}
#        MessageFacility
        ${MF_MESSAGELOGGER}
        ${MF_UTILITIES}
)

simple_plugin(STIBDQM "module"
	otsdaq-fermilabtestbeam_Overlays
	     DataDecoders
  NetworkUtilities
	artdaq-core_Data
	${CETLIB_EXCEPT}
	${ROOT_CORE}
	${ROOT_GPAD}
	${ROOT_HIST}
	${ROOT_GUI}
	${ROOT_RIO}
  ${ROOT_NET}
#	MessageFacility
	${MF_MESSAGELOGGER}
	${MF_UTILITIES}
)

simple_plugin(TriggerCounter "module"
	otsdaq-fermilabtestbeam_Overlays
  NetworkUtilities
	artdaq-core_Data
	${CETLIB_EXCEPT}
	${ROOT_GPAD}
	${ROOT_HIST}
	${ROOT_GUI}
	${ROOT_RIO}
  ${ROOT_NET}
#	MessageFacility
	${MF_MESSAGELOGGER}
	${MF_UTILITIES}
)

simple_plugin(FTBFInput "source"
  artdaq_ArtModules_ArtdaqSharedMemoryService_service
  artdaq_DAQdata
  otsdaq-fermilabtestbeam_Overlays
  artdaq-utilities_Plugins
  artdaq-core_Core
  artdaq-core_Utilities
  rt
  art_Framework_Services_Registry
  art_Framework_Core
  art_Framework_IO_Sources
  art_Framework_Principal
  art_Persistency_Provenance
  art_Utilities
  MF_MessageLogger
)

simple_plugin(EventDataServer "module"
otsdaq-fermilabtestbeam_Overlays
artdaq_DAQrate
artdaq_DAQdata
artdaq-core_Data
artdaq-core_Core
MF_MessageLogger
${Boost_SYSTEM_LIBRARY}
${Boost_THREAD_LIBRARY}
pthread
)
