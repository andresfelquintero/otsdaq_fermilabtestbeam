#ifndef ots_DACsTableBase_h
#define ots_DACsTableBase_h

#include "otsdaq-fermilabtestbeam/DetectorConfiguration/ROCDACs.h"

#include <map>
#include <string>

namespace ots
{
class DACsTableBase
{
  public:
	DACsTableBase(std::string  configurationName,
	              unsigned int rocNameColumn,
	              unsigned int firstDAC,
	              unsigned int lastDAC);
	virtual ~DACsTableBase(void);

	// Getters
	const ROCDACs& getROCDACs(std::string rocName) const;

  protected:
	std::map<std::string, unsigned int> dacNameToDACAddress_;
	std::map<std::string, ROCDACs>      nameToROCDACsMap_;
	const unsigned int                  rocNameColumn_;
	const unsigned int                  firstDAC_;
	const unsigned int                  lastDAC_;
};
}
#endif
