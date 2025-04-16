#ifndef _ots_FSSRROCDEfinitions_h_
#define _ots_FSSRROCDEfinitions_h_

#include "otsdaq-fermilabtestbeam/DetectorHardware/VROCDefinitions.h"
//#include "otsdaq/BitManipulator/BitManipulator.h"
//#include "otsdaq/DAQHardware/FSSRFirmwareDefinitions.h"
//#include "otsdaq/DAQHardware/FSSRFirmware.h"
//#include "otsdaq/DataTypes/DataTypes.h"
//#include "otsdaq/ConfigurationInterface/ROCStream.h"

#include <stdint.h>
#include <string>

namespace ots
{
class FSSRROCDefinitions : public VROCDefinitions
{
  public:
	FSSRROCDefinitions();
	virtual ~FSSRROCDefinitions(void);

	static uint64_t makeDACWriteHeader(
	    int                chipId,
	    const std::string& registerName);  // Does not work for SendData and RejectHits
	static uint64_t makeDACSetHeader(
	    int                chipId,
	    const std::string& registerName);  // Only works for SendData and RejectHits
	static uint64_t makeDACResetHeader(
	    int                chipId,
	    const std::string& registerName);  // Only works for SendData and RejectHits
	static uint64_t makeDACReadHeader(
	    int                chipId,
	    const std::string& registerName);  // Does not work for SendData and RejectHits
	static uint64_t    makeDACWriteCommand(int                chipId,
	                                       const std::string& registerName,
	                                       unsigned int       valueToWrite);
	static uint64_t    makeDACReadCommand(int chipId, const std::string& registerName);
	static std::string makeMaskWriteCommand(int                chipId,
	                                        const std::string& registerName,
	                                        std::string        valueToWrite);
	static uint64_t    makeMaskReadCommand(int chipId, const std::string& registerName);

	uint64_t setSendData(int chipId);
	uint64_t resetSendData(int chipId);
	uint64_t setRejectHits(int chipId);
	uint64_t resetRejectHits(int chipId);

  private:
	static uint64_t     makeDACHeader(int                chipId,
	                                  const std::string& registerName,
	                                  unsigned int       instruction);
	static unsigned int getDACRegisterAddress(const std::string& registerName);
	enum
	{
		write      = 1,
		set        = 2,
		read       = 4,
		reset      = 5,
		setDefault = 6
	};
};
}

#endif
