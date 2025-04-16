link_directories($ENV{XDAQ_LIB})



cet_make_exec(FermilabTestBeamInterfaceTestMain SOURCE FermilabTestBeamInterfaceTestMain.cc
  LIBRARIES
		ConfigurationInterface
		DAQHardware
		FECore
		TableCore
        WorkLoopManager
		NetworkUtilities
		Macros
		xx_usb
		artdaq_DAQdata
		$ENV{XDAQ_LIB}/libxdaq.so
		$ENV{XDAQ_LIB}/libconfig.so
		$ENV{XDAQ_LIB}/libpeer.so
		$ENV{XDAQ_LIB}/libtoolbox.so
		$ENV{XDAQ_LIB}/liblog4cplus.so
		$ENV{XDAQ_LIB}/libcgicc.so
		$ENV{XDAQ_LIB}/libxcept.so
		$ENV{XDAQ_LIB}/libxoap.so
		$ENV{XDAQ_LIB}/libxdata.so
		$ENV{XDAQ_LIB}/libxgi.so
		$ENV{XDAQ_LIB}/liblogudpappender.so
		$ENV{XDAQ_LIB}/liblogxmlappender.so
		$ENV{XDAQ_LIB}/libmimetic.so
		$ENV{XDAQ_LIB}/libasyncresolv
		$ENV{XDAQ_LIB}/libb2innub.so
        $ENV{ARTDAQ_DATABASE_LIB}/libbsoncxx.so
		${XDAQ_XERCES-C}
		${Boost_SYSTEM_LIBRARY}
		/lib64/libuuid.so.1
		usb
		pthread
		MF_MessageLogger
)

install_headers()
install_source()
