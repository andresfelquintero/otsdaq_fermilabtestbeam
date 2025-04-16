#include <fstream>
#include <iostream>

<<<<<<< HEAD
#include "otsdaq/ConfigurationInterface/ConfigurationManager.h"
//#include "otsdaq/ConfigurationDataFormats/ConfigurationGroupKey.h"
#include "otsdaq/ConfigurationInterface/ConfigurationInterface.h"
=======
#include "otsdaq/ConfigurationInterface/ConfigurationInterface.h"
#include "otsdaq/ConfigurationInterface/ConfigurationManager.h"
#include "otsdaq/TableCore/TableGroupKey.h"
>>>>>>> ca925295410afb3e1f08a9d6dc0e002da27372af
//#include "otsdaq-fermilabtestbeam/FEInterfaces/FSSRInterface_interface.cc"
//#include "otsdaq-fermilabtestbeam/FEInterfaces/FEPixelTelescopeInterface_interface.cc"
#include "otsdaq-fermilabtestbeam/FEInterfaces/FECAMACInterface_interface.cc"

int main(int argc, char** argv)
{
	// Variables
	const int          supervisorInstance_    = 1;
	const unsigned int configurationKeyValue_ = 49;  // atoi(argv[1]);

	ConfigurationManager* theConfigurationManager_ = new ConfigurationManager();

	std::string configurationGroupName = "T992Config";
	std::string XDAQContextTableName_ =
	    "XDAQContextTable";                        // VERY TOP BRANCH NAME OF THE CONTEXT
	std::string contextUID_ = "CherenkovContext";  // ContextUID in table XDAQContextTable
	std::string applicationUID_ =
	    "CherenkovFESupervisor";  // ApplicationUID in XDAQApplicationTable
	std::string feInterfaceUID_ =
	    "CherenkovFE";  // FEInterfaceUID in FEInterfaceConfiguration

	std::string supervisorConfigurationPath_ = "/" + contextUID_ +
	                                           "/LinkToApplicationTable/" +
	                                           applicationUID_ + "/LinkToSupervisorTable";

	const ConfigurationTree theXDAQContextConfigTree_ =
	    theConfigurationManager_->getNode(XDAQContextTableName_);

	std::pair<std::string, TableGroupKey> theGroup(configurationGroupName,
	                                               TableGroupKey(configurationKeyValue_));

	// theConfigurationManager_->loadTableGroup(theGroup.first, theGroup.second,
	// true);

	//	FSSRInterface* theInterface_ = new FSSRInterface(
	//			feInterfaceUID_,
	//						   theXDAQContextConfigTree_,
	//						   supervisorConfigurationPath_       +
	//						   "/LinkToFEInterfaceTable/" +
	//						   feInterfaceUID_                      +
	//						   "/LinkToFETypeTable");

	//	FEPixelTelescopeInterface* theInterface_ = new FEPixelTelescopeInterface(
	//						   feInterfaceUID_,
	//						   theXDAQContextConfigTree_,
	//						   supervisorConfigurationPath_       +
	//						   "/LinkToFEInterfaceTable/" +
	//						   feInterfaceUID_                      +
	//						   "/LinkToFETypeTable");

	FECAMACInterface* theInterface_ =
	    new FECAMACInterface(feInterfaceUID_,
	                         theXDAQContextConfigTree_,
	                         supervisorConfigurationPath_ + "/LinkToFEInterfaceTable/" +
	                             feInterfaceUID_ + "/LinkToFETypeTable");

	std::cout << "Done with new" << std::endl;
	// Test interface class methods here //
	theInterface_->configure();
	std::cout << "Configured" << std::endl;
	theInterface_->start(std::string(argv[1]));
	std::cout << "Started" << std::endl;
	unsigned int second  = 5;  // x100us
	unsigned int time    = 2;
	unsigned int counter = 0;
	while(counter++ < time)
	{
		theInterface_->running();  // There is a 1ms sleep inside the running
		                           // std::cout << counter << std::endl;
		usleep(100);
	}
	std::cout << "Ran." << std::endl;
	theInterface_->stop();
	std::cout << "Stopped." << std::endl;
	theInterface_->halt();
	delete theInterface_;
	theInterface_ =
	    new FECAMACInterface(feInterfaceUID_,
	                         theXDAQContextConfigTree_,
	                         supervisorConfigurationPath_ + "/LinkToFEInterfaceTable/" +
	                             feInterfaceUID_ + "/LinkToFETypeTable");
	theInterface_->configure();

	return 0;
}
