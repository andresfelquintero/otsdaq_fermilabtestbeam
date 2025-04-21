find_package(ROOT REQUIRED COMPONENTS Core Hist Graf RIO Tree)
include_directories(${ROOT_INCLUDE_DIRS})

cet_make(LIBRARY_NAME FermilabTestbeamDQMHistos
         LIBRARIES
             TableCore
             ConfigurationInterface
             NetworkUtilities
             DataDecoders
             Macros
             ${ROOT_BASIC_LIB_LIST}
             ${ROOT_LIBRARIES}
        ${MF_MESSAGELOGGER}
)
install_headers()
install_source()



#cet_make(LIBRARY_NAME FermilabTestbeamDQMHistos
#		 LIBRARIES
#		 TableCore
#	     ConfigurationInterface
#	     NetworkUtilities
#	     DataDecoders
#		 Macros
#	     ${ROOT_BASIC_LIB_LIST}
#	${MF_MESSAGELOGGER}
#)

#install_headers()
#install_source()
