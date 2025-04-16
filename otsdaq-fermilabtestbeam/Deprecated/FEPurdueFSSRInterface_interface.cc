#include "otsdaq-fermilabtestbeam/FEInterfaces/FEPurdueFSSRInterface.h"
#include "otsdaq-components/DetectorConfiguration/DACStream.h"
#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"
#include <iostream>
#include <set>

using namespace ots;

//========================================================================================================================
//FEPurdueFSSRInterface::FEPurdueFSSRInterface(unsigned int name,
//		std::string daqHardwareType, std::string firmwareType, const FEWConfiguration* configuration)
//:Socket            (theConfiguration_->getInterfaceIPAddress(name), theConfiguration_->getInterfacePort(name))
//,FEVInterface     (name,daqHardwareType,firmwareType,configuration)
//,OtsUDPHardware    (theConfiguration_->getIPAddress(name), theConfiguration_->getPort(name))
//,FSSRFirmware      (theConfiguration_->getFirmwareVersion(name), "PurdueFirmwareCore")
//,theConfiguration_ ((FEWOtsUDPHardwareConfiguration*)configuration)
//
//{
//    std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Few name: " << name
//    << " Interface IP: "   << theConfiguration_->getInterfaceIPAddress(name)
//    << " Interface Port: " << theConfiguration_->getInterfacePort(name)
//    << " IP: "             << theConfiguration_->getIPAddress(name)
//    << " Port: "           << theConfiguration_->getPort(name)
//    << std::endl;
//}
//========================================================================================================================
FEPurdueFSSRInterface::FEPurdueFSSRInterface(const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& interfaceConfigurationPath)
:Socket            (
		theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostIPAddress").getValue<std::string>()
	   ,theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostPort").getValue<unsigned int>())
,FEVInterface      (interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
,OtsUDPHardware    (theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfaceIPAddress").getValue<std::string>()
				   ,theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfacePort").getValue<unsigned int>())
,FSSRFirmware      (theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("FirmwareVersion").getValue<unsigned int>(), "PurdueFirmwareCore", "FSSRPurdueFirmware")
{
//	std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Few name: " << interfaceID
//			<< " Interface IP: "   << theConfiguration_->getInterfaceIPAddress(interfaceID)
//			<< " Interface Port: " << theConfiguration_->getInterfacePort(interfaceID)
//			<< " IP: "             << theConfiguration_->getIPAddress(interfaceID)
//			<< " Port: "           << theConfiguration_->getPort(interfaceID)
//			;
	universalAddressSize_ = 4;
	universalDataSize_ = 4;
}

//========================================================================================================================
FEPurdueFSSRInterface::~FEPurdueFSSRInterface(void)
{}


//========================================================================================================================
void FEPurdueFSSRInterface::configure(void)
{
	__MOUT__ << "Setting port: " << theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<unsigned int>() << std::endl;
	FSSRFirmware::setCSRRegister(0);
	OtsUDPHardware::write(FSSRFirmware::setDataDestination(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<unsigned int>()));

//			theConfiguration_->getStreamingIPAddress(FEVInterface::getInterfaceID()),
//			theConfiguration_->getStreamingPort(FEVInterface::getInterfaceID())));

	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::configureClocks(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ClockSelect").getValue<std::string>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ClockSpeedMHz").getValue<float>()));

	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::resetDetector(),10);

    configureDetector();
	__MOUT__ << "DONE!" << std::endl;
}

//========================================================================================================================
void FEPurdueFSSRInterface::configureDetector(void)
{
	__MOUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	//__MOUT__ <<
//	for(const auto& interface: theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable").getChildren())
//	{
//		__MOUT__ << interface.first << std::endl;
//		__MOUT__ << interface.second.getNode("FEWriterDetectorAddress").getValue<unsigned int>() << std::endl;
//	}


	DACStream theDACStream;
	theDACStream.makeStream(theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable"));
	__MOUT__ << std::endl;
	//NOTE This way we first upload all registers to the FPGA and then all together to the ROCS
	std::set<unsigned int> fecChannel;
	//return;
	for(DACStream::const_iterator it=theDACStream.getChannelStreamMap().begin(); it!=theDACStream.getChannelStreamMap().end(); it++)
	{
		__MOUT__ << it->first << " UID: " <<  it->second.getDetectorID() << std::endl;
		if(it->second.getROCStatus())
		{
			//FIXME This is not an hardware task since the hardware can be used by FEW and FER
			//theHardware_.uploadDACsToFEW(it->first,it->second);//TODO Maybe add in the firmware the DAC version so there is no need to copy the DACs again
			std::vector<std::string> sendBuffer;
			std::vector<std::string> receiveBuffer;
			//string buffer;
//			for (const auto& it2: it->second.getROCDACs().getDACList())
//			{
//				std::cout << __COUT_HDR_FL__ << "Register?" << std::endl;
//				std::cout << __COUT_HDR_FL__ << "Register-1: " << it2.first << " val: " << it2.second.first  << " = " << it2.second.second << std::endl;
//			}
			FSSRFirmware::makeDACBuffer(sendBuffer, it->first, it->second);

			OtsUDPHardware::writeAndAcknowledge(sendBuffer);//,receiveBuffer);
/*
			std::vector<std::string>::iterator sendIt    = sendBuffer.begin();
			std::vector<std::string>::iterator receiveIt = receiveBuffer.begin();
			for(; sendIt!=sendBuffer.end(); sendIt++, receiveIt++)
			{
				std::string toFix = FSSRFirmware::compareSendAndReceive(*sendIt, *receiveIt);
				if(!toFix.empty())
				{
					std::string receivedFixed;
					OtsUDPHardware::read(toFix,receivedFixed);
					std::string toFixAgain = FSSRFirmware::compareSendAndReceive(toFix, receivedFixed);
					if(!toFixAgain.empty())
					{
						std::cout << __COUT_HDR_FL__ << "ERROR: I tried to re-send " << std::endl;

						for (unsigned int i = 0; i < toFixAgain.size(); i++)
							std::cout << __COUT_HDR_FL__ << (unsigned int)toFixAgain[i] << std::endl;

						std::cout << __COUT_HDR_FL__ << " and got an error again!" << std::endl;
					}
				}
			}
*/
			std::string maskBuffer;
			FSSRFirmware::makeMaskBuffer(maskBuffer, it->first, it->second);
			OtsUDPHardware::writeAndAcknowledge(maskBuffer);
			fecChannel.insert(it->first);
			//conbinedBuffer += buffer;
		}
	}

	//FIXME This is not an hardware task since the hardware can be used by FEW and FER
	//    for(set<unsigned int>::const_iterator it=fecChannel.begin(); it!=fecChannel.end(); it++)
	//uploadDACsToDetector(*it);
}

//========================================================================================================================
void FEPurdueFSSRInterface::halt(void)
{
	std::cout << __COUT_HDR_FL__ << "\tHalt" << std::endl;
	stop();
}

//========================================================================================================================
void FEPurdueFSSRInterface::pause(void)
{
	std::cout << __COUT_HDR_FL__ << "\tPause" << std::endl;
	stop();
}

//========================================================================================================================
void FEPurdueFSSRInterface::resume(void)
{
	std::cout << __COUT_HDR_FL__ << "\tResume" << std::endl;
	start("");
}
//========================================================================================================================
void FEPurdueFSSRInterface::start(std::string )//runNumber)
{
	std::cout << __COUT_HDR_FL__ << "\tStart" << std::endl;
	__MOUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	std::string csrRegisterRead;
	std::string csrRegisterBuffer = FSSRFirmware::readCSRRegister();

	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), csrRegisterRead);
	uint32_t csrRegisterValue = FSSRFirmware::createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware::setCSRRegister(csrRegisterValue);
	std::cout << __COUT_HDR_FL__ << "STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
//
	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::enableTrigger());
//	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::resetBCO());
	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::armBCOReset());
	OtsUDPHardware::write(FSSRFirmware::startStream(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus0").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus1").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus2").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus3").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus4").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus5").getValue<bool>()

	));
	//WARNING!!!!!!!!!!
	//WARNING!!!!!!!!!!
	//The previous is the last command that I send before start taking data and when I use writeAndAcknowledge is not sending me back anything!
	//If I add a read or anything after that it looks like that firmware is not able to cope and I overwrite the write command so
	//DON'T ADD ANYTHING AFTER THIS OR USE A usleep!!!!!
}

//========================================================================================================================
bool FEPurdueFSSRInterface::running(void)
{
	std::cout << __COUT_HDR_FP__ << std::endl;
	__MOUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	std::string csrRegisterRead;
	std::string csrRegisterBuffer = FSSRFirmware::readCSRRegister();

	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), csrRegisterRead);
	uint32_t csrRegisterValue = FSSRFirmware::createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware::setCSRRegister(csrRegisterValue);
	std::cout << __COUT_HDR_FL__ << "STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	if(csrRegisterValue&0x00080000)
	{
		usleep(500000);
		return true;
	}

	OtsUDPHardware::writeAndAcknowledge(FSSRFirmware::resetBCO());
	return false;
}

//========================================================================================================================
void FEPurdueFSSRInterface::stop(void)
{
	std::cout << __COUT_HDR_FL__ << "\tStop" << std::endl;
	OtsUDPHardware::write(FSSRFirmware::stopStream());
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for returnValue must be max UDP size to handle return possibility
int ots::FEPurdueFSSRInterface::universalRead(char *address, char *returnValue)
{
    std::cout << __COUT_HDR_FL__ << "address size " << universalAddressSize_ << std::endl;

    std::cout << __COUT_HDR_FL__ << "Request: ";
	for(unsigned int i=0;i<universalAddressSize_;++i)
		printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

	std::string readBuffer(universalDataSize_,0); //0 fill to correct number of bytes
    std::cout << __COUT_HDR_FL__ << "Command: " << FSSRFirmware::universalRead(address) << std::endl;

    OtsUDPHardware::read(FSSRFirmware::universalRead(address), readBuffer); // data reply

    std::cout << __COUT_HDR_FL__ << "Result SIZE: " << readBuffer.size() << std::endl;
    std::memcpy(returnValue,readBuffer.substr(8).c_str(),universalDataSize_);
    return 0;
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::FEPurdueFSSRInterface::universalWrite(char* address, char* writeValue)
{

    std::cout << __COUT_HDR_FL__ << "address size " << universalAddressSize_ << std::endl;
    std::cout << __COUT_HDR_FL__ << "data size " << universalDataSize_ << std::endl;
    std::cout << __COUT_HDR_FL__ << "Sending: ";
    for(unsigned int i=0;i<universalAddressSize_;++i)
    	printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

    OtsUDPHardware::write(FSSRFirmware::universalWrite(address,writeValue)); // data request
}

DEFINE_OTS_INTERFACE(FEPurdueFSSRInterface)
