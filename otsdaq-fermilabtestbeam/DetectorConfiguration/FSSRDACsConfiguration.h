#ifndef _ots_FSSRDACsConfiguration_h_
#define _ots_FSSRDACsConfiguration_h_

#include "otsdaq-fermilabtestbeam/DetectorConfiguration/ROCDACs.h"

namespace ots
{
class ConfigurationTree;

class FSSRDACsConfiguration
{
  public:
	FSSRDACsConfiguration(void);
	virtual ~FSSRDACsConfiguration(void);

	const ROCDACs getROCDACs(const ConfigurationTree& dacsConfiguration);

  private:
	std::map<std::string, unsigned int> dacNameToDACAddress_;
};
}
#endif
