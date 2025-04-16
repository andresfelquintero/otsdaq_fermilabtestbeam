basic_plugin(FSSRDQMHistosConsumer "processor"
    Configurable
	DataManager
	RootUtilities
	FermilabTestbeamDQMHistos
        WorkLoopManager
	${CETLIB}
	MF_MessageLogger
  )

basic_plugin(FSSRRawDataSaverConsumer "processor"
	DataManager
        WorkLoopManager
	${CETLIB}
	MF_MessageLogger
  )

basic_plugin(WireChamberDQMHistosConsumer "processor"
    Configurable
	DataManager
	RootUtilities
	FermilabTestbeamDQMHistos
        WorkLoopManager
	${CETLIB}
  )

basic_plugin(WireChamberDataSaverConsumer "processor"
	DataManager
        WorkLoopManager
	${CETLIB}
	${MF_MESSAGELOGGER}
  )

basic_plugin(RootDQMHistosConsumer "processor"
    Configurable
    RootUtilities
    DataManager
    WorkLoopManager
    ConfigurationInterface
 	  Macros
	  ${ROOT_BASIC_LIB_LIST}
	  ${MF_MESSAGELOGGER}
  )

basic_plugin(UDPFSSRDataListenerProducer "processor"
	DataManager
	NetworkUtilities
	${CETLIB}
  )

basic_plugin(UDPFSSRDataStreamerConsumer "processor"
	DataManager
	NetworkUtilities
	${CETLIB}
  )


install_headers()
install_source()
