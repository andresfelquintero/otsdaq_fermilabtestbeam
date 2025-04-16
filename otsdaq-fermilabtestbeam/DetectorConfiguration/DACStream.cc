#include "otsdaq-fermilabtestbeam/DetectorConfiguration/DACStream.h"
#include "otsdaq-fermilabtestbeam/DetectorConfiguration/FSSRDACsConfiguration.h"
#include "otsdaq/ConfigurationInterface/ConfigurationTree.h"

using namespace ots;

//==============================================================================
DACStream::DACStream(void) {}

//==============================================================================
DACStream::~DACStream(void) {}

//==============================================================================
void DACStream::makeStream(const ConfigurationTree& feToDetectorTree)
{
	theChannelStreamMap_.clear();
	std::multimap<unsigned int, ROCStream>::iterator currentElement;
	std::string                                      detectorType = "";
	auto feWriterDetectorList = feToDetectorTree.getChildren();

	FSSRDACsConfiguration dacsMaker;
	for(auto& it : feWriterDetectorList)
	{
		const ConfigurationTree& detectorConfiguration =
		    it.second.getNode("LinkToDetectorTable");
		const ConfigurationTree& dacsConfiguration =
		    detectorConfiguration.getNode("LinkToDACsTable");
		const ConfigurationTree& maskConfiguration =
		    detectorConfiguration.getNode("LinkToMaskTable");
		//__COUT__ << feToDetectorTree << std::endl;
		//__COUT__ << detectorConfiguration << std::endl;
		//__COUT__ << dacsConfiguration << std::endl;
		//__COUT__ << dacsConfiguration << std::endl;
		currentElement = theChannelStreamMap_.insert(std::pair<unsigned int, ROCStream>(
		    it.second.getNode("FEChannel").getValue<unsigned int>(), ROCStream()));
		currentElement->second.setDetectorID(it.first);
		detectorType =
		    detectorConfiguration.getNode("DetectorType").getValue<std::string>();
		currentElement->second.setROCType(detectorType);
		currentElement->second.setROCStatus(
		    detectorConfiguration.getNode(TableViewColumnInfo::COL_NAME_STATUS)
		        .getValue<bool>());
		currentElement->second.setFEWROCAddress(
		    it.second.getNode("ROCAddress").getValue<unsigned int>());
		currentElement->second.setROCDACs(dacsMaker.getROCDACs(dacsConfiguration));
		//		__COUT__ << "MASK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		//		__COUT__ << maskConfiguration.getNode("KillMask").getValue<std::string>()
		//<<  std::endl;
		currentElement->second.setROCMask(
		    maskConfiguration.getNode("KillMask").getValue<std::string>());
	}
}
