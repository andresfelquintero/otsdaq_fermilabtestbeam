#ifndef _ots_UDPFSSRDataStreamerConsumer_h_
#define _ots_UDPFSSRDataStreamerConsumer_h_

#include "otsdaq/NetworkUtilities/UDPDataStreamerBase.h"
#include "otsdaq/DataManager/DataConsumer.h"
#include "otsdaq/Configurable/Configurable.h"

#include <string>

namespace ots
{

class ConfigurationTree;

class UDPFSSRDataStreamerConsumer : public UDPDataStreamerBase, public DataConsumer, public Configurable
{
public:
	UDPFSSRDataStreamerConsumer(std::string supervisorApplicationUID, std::string bufferUID, std::string processorUID,  const ConfigurationTree& theXDAQContextConfigTree, const std::string& configurationPath);
	virtual ~UDPFSSRDataStreamerConsumer(void);

protected:
	bool workLoopThread(toolbox::task::WorkLoop* workLoop);

	void fastRead(void);
	void slowRead(void);

	//For fast read
	std::string*                          dataP_;
	std::map<std::string,std::string>*    headerP_;
	//For slow read
	std::string                           data_;
	std::map<std::string,std::string>     header_;

};

}

#endif
