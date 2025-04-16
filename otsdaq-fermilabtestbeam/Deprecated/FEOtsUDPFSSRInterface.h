#ifndef _ots_FEOtsUDPFSSRInterface_h_
#define _ots_FEOtsUDPFSSRInterface_h_

#include "otsdaq/FECore/FEVInterface.h"
#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"
#include "otsdaq-components/DAQHardware/FSSRFirmware.h"
#include "otsdaq-components/DAQHardware/OtsUDPFirmwareCore.h"

#include <string>

namespace ots
{

class FEOtsUDPFSSRInterface : public FEVInterface, public OtsUDPHardware, public FSSRFirmware, public OtsUDPFirmwareCore
	{

public:
    //FEOtsUDPFSSRInterface     (unsigned int name=0, std::string daqHardwareType="daqHardwareType",	std::string firmwareType="firmwareType", const FEInterfaceConfigurationBase* configuration=0);
    FEOtsUDPFSSRInterface     (const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& interfaceConfigurationPath);
    virtual ~FEOtsUDPFSSRInterface(void);

    void configure        (void);
    void halt             (void);
    void pause            (void);
    void resume           (void);
    void start            (std::string runNumber) override;
    void stop             (void);

    bool running          (void);

    int universalRead	  (char* address, char* readValue) override;
    void universalWrite	  (char* address, char* writeValue) override;

    //void configureFEW     (void);
    void configureDetector(void);

private:
    //void destroy          (void);
};

}

#endif
