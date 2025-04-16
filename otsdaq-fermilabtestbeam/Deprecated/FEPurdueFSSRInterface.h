#ifndef _ots_FEPurdueFSSRInterface_h_
#define _ots_FEPurdueFSSRInterface_h_

#include "otsdaq/FECore/FEVInterface.h"
#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"
#include "otsdaq-components/DAQHardware/FSSRFirmware.h"

namespace ots
{

class FEPurdueFSSRInterface : public FEVInterface, public OtsUDPHardware, public FSSRFirmware
{

public:
	FEPurdueFSSRInterface     (const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& interfaceConfigurationPath);
	virtual ~FEPurdueFSSRInterface(void);

	//void destroy          (void);
	//void configureFE     (void);
	void configureDetector(void);

	void configure        (void);
	void halt             (void);
	void pause            (void);
	void resume           (void);
	void start            (std::string runNumber);
	void stop             (void);
	bool running          (void);

	int universalRead	  (char *address, char *returnValue) override;
	void universalWrite	  (char *address, char *writeValue) override;

private:

};

}

#endif
