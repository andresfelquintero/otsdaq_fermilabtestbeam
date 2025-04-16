#include "otsdaq-fermilabtestbeam/FEInterfaces/FECommanderInterface.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>  //for memcpy
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

using namespace ots;

//========================================================================================================================
ots::FECommanderInterface::FECommanderInterface(
    const std::string&       interfaceUID,
    const ConfigurationTree& theXDAQContextConfigTree,
    const std::string&       interfaceConfigurationPath)
    : Socket(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostIPAddress")
                 .getValue<std::string>(),
             theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                 .getNode("HostPort")
                 .getValue<unsigned int>())
    , FEVInterface(interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
    , interfaceSocket_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                           .getNode("InterfaceIPAddress")
                           .getValue<std::string>(),
                       theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                           .getNode("InterfacePort")
                           .getValue<unsigned int>())
    , stateMachineName_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                            .getNode("StateMachineName")
                            .getValue<std::string>())
    , configurationAlias_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
                              .getNode("ConfigurationAlias")
                              .getValue<std::string>())
{
	Socket::initialize();
	universalAddressSize_ = 8;
	universalDataSize_    = 8;
}

//========================================================================================================================
ots::FECommanderInterface::~FECommanderInterface(void) {}

//========================================================================================================================
void ots::FECommanderInterface::send(std::string buffer)
{
	try
	{
		bool verbose = false;
		std::cout << __PRETTY_FUNCTION__ << "Sending: " << buffer << std::endl;
		if(TransceiverSocket::send(interfaceSocket_, buffer, verbose) < 0)
		{
			__SS__ << "Write failed to IP:Port " << interfaceSocket_.getIPAddress() << ":"
			       << interfaceSocket_.getPort() << __E__;
			__SS_THROW__;
		}
	}
	catch(std::runtime_error& e)
	{
		throw;
	}
	catch(...)
	{
		__SS__ << "Unrecognized exception caught!" << std::endl;
		__SS_THROW__;
	}
}
//========================================================================================================================
void ots::FECommanderInterface::halt(void)
{
	// MESSAGE = "PhysicsRuns0,Halt"
	send(stateMachineName_ + ",Halt");
}

//========================================================================================================================
void ots::FECommanderInterface::pause(void) { send(stateMachineName_ + ",Pause"); }

//========================================================================================================================
void ots::FECommanderInterface::resume(void) { send(stateMachineName_ + ",Resume"); }
//========================================================================================================================
void ots::FECommanderInterface::start(std::string runNumber)
{
	// MESSAGE = "PhysicsRuns0,Start, %i" % (int(run)) #"PhysicsRuns0,Start"
	send(stateMachineName_ + ",Start," + runNumber);
}

//========================================================================================================================
void ots::FECommanderInterface::stop(void)
{
	// MESSAGE = "PhysicsRuns0,Stop"
	send(stateMachineName_ + ",Stop");
}

//========================================================================================================================
void ots::FECommanderInterface::configure(void)
{
	std::cout << __COUT_HDR__ << "\tConfigure" << std::endl;
	// MESSAGE = "PhysicsRuns0,Configure,FQNETConfig"
	send(stateMachineName_ + ",Configure," + configurationAlias_);
}

DEFINE_OTS_INTERFACE(ots::FECommanderInterface)
