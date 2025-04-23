#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#this was included for the package to work properly, only the following two lines and one of the libraries caled Root_Libraries
find_package(ROOT REQUIRED COMPONENTS Core Hist Graf RIO Tree)
include_directories(${ROOT_INCLUDE_DIRS})

include(CetRootCint)
cet_rootcint(EventBuilder)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
cet_make_library(LIBRARY_NAME EventBuilder
                 SOURCE ${CMAKE_CURRENT_BINARY_DIR}/EventBuilderCint.cc
		        Event.cxx
			AssociativeMemoryEventBuilder.cc
			EventDataSaver.cc
			VirtualEventBuilder.cc
		LIBRARIES
		        ${ROOT_BASIC_LIB_LIST}
	                ${ROOT_LIBRARIES}
			WorkLoopManager
			DataManager
			DataDecoders
               )

install_headers()
install_source()
