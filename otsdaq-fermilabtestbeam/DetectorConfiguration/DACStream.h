#ifndef _ots_DACStream_h_
#define _ots_DACStream_h_

#include "otsdaq-fermilabtestbeam/DetectorConfiguration/ROCStream.h"

#include <map>
#include <string>

namespace ots
{
// class DetectorConfiguration;
// class DetectorToFEConfiguration;
// class DACsTableBase;
// class MaskConfiguration;
class ConfigurationManager;
class ConfigurationTree;

class DACStream  // ConfigurationStream (makeDACStream, makeMaskStream)
{
  public:
	typedef std::multimap<unsigned int, ROCStream> ROCStreams;
	typedef ROCStreams::iterator                   iterator;
	typedef ROCStreams::const_iterator             const_iterator;

	DACStream(void);
	virtual ~DACStream(void);

	void makeStream(const ConfigurationTree& feToDetectorTree);

	// Iterators
	iterator       begin(void) { return theChannelStreamMap_.begin(); }
	iterator       end(void) { return theChannelStreamMap_.end(); }
	const_iterator begin(void) const { return theChannelStreamMap_.begin(); }
	const_iterator end(void) const { return theChannelStreamMap_.end(); }

	const std::multimap<unsigned int, ROCStream>& getChannelStreamMap() const
	{
		return theChannelStreamMap_;
	}

  private:
	//            fec channel
	std::multimap<unsigned int, ROCStream> theChannelStreamMap_;
	// std::map<std::string, TableVersion>   	TableVersions_;
};
}

#endif  // ots_DACStream_h
