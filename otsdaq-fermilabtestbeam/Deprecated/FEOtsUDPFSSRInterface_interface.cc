#include "otsdaq-fermilabtestbeam/FEInterfaces/FEOtsUDPFSSRInterface.h"
#include "otsdaq-components/DetectorConfiguration/DACStream.h"
#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"
#include <iostream>
#include <set>
#include <unistd.h>

using namespace ots;

//========================================================================================================================
//FEOtsUDPFSSRInterface::FEOtsUDPFSSRInterface(unsigned int name,
//		std::string daqHardwareType, std::string firmwareType, const FEWConfiguration* configuration)
//:Socket            (theConfiguration_->getInterfaceIPAddress(name), theConfiguration_->getInterfacePort(name))
//,FEVInterface     (name,daqHardwareType,firmwareType,configuration)
//,OtsUDPHardware    (theConfiguration_->getIPAddress(name), theConfiguration_->getPort(name))
//,FSSRFirmware      (theConfiguration_->getFirmwareVersion(name), "PurdueFirmwareCore")
//,theConfiguration_ ((FEWOtsUDPHardwareConfiguration*)configuration)
//
//{
//    __COUT__ << __PRETTY_FUNCTION__ << "Few name: " << name
//    << " Interface IP: "   << theConfiguration_->getInterfaceIPAddress(name)
//    << " Interface Port: " << theConfiguration_->getInterfacePort(name)
//    << " IP: "             << theConfiguration_->getIPAddress(name)
//    << " Port: "           << theConfiguration_->getPort(name)
//    << std::endl;
//}
//========================================================================================================================
FEOtsUDPFSSRInterface::FEOtsUDPFSSRInterface(const std::string& interfaceUID, const ConfigurationTree& theXDAQContextConfigTree, const std::string& interfaceConfigurationPath)
:Socket            (
		theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostIPAddress").getValue<std::string>()
	   ,theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("HostPort").getValue<unsigned int>())
,FEVInterface      (interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath)
,OtsUDPHardware    (
		theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfaceIPAddress").getValue<std::string>()
		, theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("InterfacePort").getValue<unsigned int>()
		, -1 /*version*/
		, false /*verbose*/)
,FSSRFirmwareBase  (theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("FirmwareType").getValue<std::string>(),
					theXDAQContextConfigTree.getNode(interfaceConfigurationPath).getNode("FirmwareVersion").getValue<unsigned int>())
{
//    __COUT__ << "FEW name: " << interfaceID
//			<< " Interface IP: "   << theConfiguration_->getInterfaceIPAddress(interfaceID)
//			<< " Interface Port: " << theConfiguration_->getInterfacePort(interfaceID)
//			<< " IP: "             << theConfiguration_->getIPAddress(interfaceID)
//			<< " Port: "           << theConfiguration_->getPort(interfaceID)
//		      ;
	universalAddressSize_ = 8;
	universalDataSize_    = 8;
}

//========================================================================================================================
FEOtsUDPFSSRInterface::~FEOtsUDPFSSRInterface(void)
{}


//========================================================================================================================
void FEOtsUDPFSSRInterface::configure(void)
{
	__COUT__ << "Clearing receive socket buffer: " << OtsUDPHardware::clearReadSocket() << " packets cleared." << std::endl;
	__COUT__ << "Setting data destination to port: " << theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<unsigned int>()  << std::endl;


	std::string writeBuffer;
	std::string readBuffer;
	uint64_t tmp;

	//FIXME -- for now set burst destination mac/ip address to that of this FEW interface
//	OtsUDPFirmware::setBurstDestinationAsSelf(writeBuffer);
//	__COUT__ << "setBurstDestinationAsSelf " <<	writeBuffer.size() << std::endl;
//	OtsUDPHardware::write(writeBuffer);
//
//
//	__COUT__ << "Destination IP: " << theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>();
//	sscanf(theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>().c_str(), "%*d.%*d.%*d.%lu",&tmp);
//	__COUT__ << "Destination IP Last Byte: " << tmp  << std::endl;


	// set POR
//	__COUT__ << "Destination Port: " <<
//			theConfiguration_->getStreamingPort(FEVInterface::getInterfaceID()) ;
	writeBuffer.resize(0);
	OtsUDPFirmwareCore::setDataDestination(writeBuffer,
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToIPAddress").getValue<std::string>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("StreamToPort").getValue<uint64_t>()
			);
	OtsUDPHardware::write(writeBuffer);


	__COUT__ << "Reading back burst dest MAC/IP/Port: "  << std::endl;
	writeBuffer.resize(0);
	OtsUDPFirmwareCore::readDataDestinationMAC(writeBuffer);
	OtsUDPHardware::read(writeBuffer,readBuffer);
	__COUT__ << "RECEIVED MESSAGE: ";
	for(uint32_t i=0; i<readBuffer.size(); i++)
		printf("%2.2X-",(((int16_t)readBuffer[i])&0xFF));
	std::cout << std::endl;
	writeBuffer.resize(0);
	OtsUDPFirmwareCore::readDataDestinationIP(writeBuffer);
	OtsUDPHardware::read(writeBuffer,readBuffer);
	__COUT__ << "RECEIVED MESSAGE: ";
	for(uint32_t i=0; i<readBuffer.size(); i++)
		printf("%2.2X-",(((int16_t)readBuffer[i])&0xFF));
	std::cout << std::endl;
	writeBuffer.resize(0);
	OtsUDPFirmwareCore::readDataDestinationPort(writeBuffer);
	OtsUDPHardware::read(writeBuffer,readBuffer);
	__COUT__ << "RECEIVED MESSAGE: ";
	for(uint32_t i=0; i<readBuffer.size(); i++)
		printf("%2.2X-",(((int16_t)readBuffer[i])&0xFF));
	std::cout << std::endl;


	__COUT__ << "Done with configuring Ethernet block."  << std::endl;
	/////////////////////////////////////////////////
	/////////////////////////////////////////////////
	__COUT__ << "Configuring FSSR..."  << std::endl;

	FSSRFirmware::setCSRRegister(0);

	__COUT__ << std::endl;

	writeBuffer.resize(0);
	FSSRFirmware::resetDCM(writeBuffer);
	OtsUDPHardware::write(writeBuffer);

	__COUT__ << std::endl;

	writeBuffer.resize(0);
	FSSRFirmware::alignReadOut(writeBuffer, 0x1e);
	OtsUDPHardware::write(writeBuffer);
	OtsUDPHardware::write(FSSRFirmware::resetDetector());

	__COUT__ << std::endl;

	configureDetector();

	__COUT__ << "DONE!"  << std::endl;
}

//========================================================================================================================
void FEOtsUDPFSSRInterface::configureDetector(void)
{
	__COUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	//__COUT__ <<
//	for(const auto& interface: theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable").getChildren())
//	{
//		__COUT__ << interface.first << std::endl;
//		__COUT__ << interface.second.getNode("FEWriterDetectorAddress").getValue<unsigned int>() << std::endl;
//	}

	__COUT__ << std::endl;

	DACStream theDACStream;
	theDACStream.makeStream(theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable"));
	__COUT__ << std::endl;
	//NOTE This way we first upload all registers to the FPGA and then all together to the ROCS
	std::set<unsigned int> fecChannel;
	bool anyOn = false;

	__COUT__ << std::endl;
	for(DACStream::const_iterator it=theDACStream.getChannelStreamMap().begin(); it!=theDACStream.getChannelStreamMap().end(); it++)
	{
		if(it->second.getROCStatus())
		{
			anyOn = true;
			//FIXME This is not an hardware task since the hardware can be used by FEW and FER
			//theHardware_.uploadDACsToFEW(it->first,it->second);//TODO Maybe add in the firmware the DAC version so there is no need to copy the DACs again
			//std::vector<std::string> sendBuffer;
			//std::vector<std::string> receiveBuffer;
			std::vector<std::string> sendBuffer;
			//std::string sendBuffer;
			//std::string receiveBuffer;
			//string buffer;
			FSSRFirmware::makeDACBuffer(sendBuffer, it->first, it->second);

			OtsUDPHardware::write(sendBuffer);
			usleep(100000);
//			OtsUDPHardware::read(sendBuffer,receiveBuffer);
//			std::vector<std::string>::iterator sendIt    = sendBuffer.begin();
//			std::vector<std::string>::iterator receiveIt = receiveBuffer.begin();
//			for(; sendIt!=sendBuffer.end(); sendIt++, receiveIt++)
//			{
//				std::string toFix = FSSRFirmware::compareSendAndReceive(*sendIt, *receiveIt);
//				if(!toFix.empty())
//				{
//					std::string receivedFixed;
//					OtsUDPHardware::read(toFix,receivedFixed);
//					std::string toFixAgain = FSSRFirmware::compareSendAndReceive(toFix, receivedFixed);
//					if(!toFixAgain.empty())
//					{
//						__COUT__ << "ERROR: I tried to re-send " << std::endl;
//
//						for (unsigned int i = 0; i < toFixAgain.size(); i++)
//							__COUT__ << (unsigned int)toFixAgain[i] << std::endl;
//
//						__COUT__ << " and got an error again!" << std::endl;
//					}
//				}
//			}

			std::string maskBuffer;
			FSSRFirmware::makeMaskBuffer(maskBuffer, it->first, it->second);
			usleep(100000);
//			OtsUDPHardware::writeAndAcknowledge(maskBuffer);
			OtsUDPHardware::write(maskBuffer);

			fecChannel.insert(it->first);
			//conbinedBuffer += buffer;
		}
	}

	__COUT__ << std::endl;

	if(!anyOn)
		__COUT__ << "NOTE: No ROCs are on!"  << std::endl;
	//FIXME This is not an hardware task since the hardware can be used by FEW and FER
	//    for(set<unsigned int>::const_iterator it=fecChannel.begin(); it!=fecChannel.end(); it++)
	//uploadDACsToDetector(*it);
}

//========================================================================================================================
void FEOtsUDPFSSRInterface::halt(void)
{
	__COUT__ << "\tHalt" << std::endl;
	stop();
}

//========================================================================================================================
void FEOtsUDPFSSRInterface::pause(void)
{
	__COUT__ << "\tPause" << std::endl;
	stop();
}

//========================================================================================================================
void FEOtsUDPFSSRInterface::resume(void)
{
	__COUT__ << "\tResume" << std::endl;
	stop();
}
//========================================================================================================================
void FEOtsUDPFSSRInterface::start(std::string )//runNumber)
{
	__COUT__ << "\tStart" << std::endl;
	__COUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	std::string value;
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), value);
	std::string CSRRegister = FSSRFirmware::readCSRRegister();
	uint32_t registerValue = FSSRFirmware::createRegisterFromValue(CSRRegister, value);
	__COUT__ << "STRIP CSR Register value:" << std::hex << registerValue << std::dec << std::endl;

	if (registerValue == 0)
	{
		__SS__ << "\tERROR - Problem with register! See above" << std::endl;
		__SS_THROW__;
	}

	FSSRFirmware::setCSRRegister(registerValue);
	value.clear();
	CSRRegister.resize(8,0);
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), value);
	__COUT__ << "STRIP CSR Register value:" << std::hex << FSSRFirmware::createRegisterFromValue(CSRRegister, value) << std::dec << std::endl;
	OtsUDPHardware::write(FSSRFirmware::enableTrigger());
	value.clear();
	CSRRegister.resize(8,0);
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), value);
	__COUT__ << "STRIP CSR Register value:" << std::hex << FSSRFirmware::createRegisterFromValue(CSRRegister, value) << std::dec << std::endl;
	OtsUDPHardware::write(FSSRFirmware::armBCOReset());
	//usleep(50000);
	//OtsUDPHardware::write(FSSRFirmware::resetBCO());
	value.clear();
	CSRRegister.resize(8,0);
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), value);
	__COUT__ << "STRIP CSR Register value BEFORE START STREAM!!:" << std::hex << FSSRFirmware::createRegisterFromValue(CSRRegister, value) << std::dec << std::endl;



	OtsUDPHardware::write(FSSRFirmware::startStream(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus0").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus1").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus2").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus3").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus4").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus5").getValue<bool>()
	));

	value.clear();
	CSRRegister.resize(8,0);
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), value);
	__COUT__ << "STRIP CSR Register value AFTER START STREAM:" << std::hex << FSSRFirmware::createRegisterFromValue(CSRRegister, value) << std::dec << std::endl;
//	OtsUDPHardware::write(OtsUDPFirmware::startBurst());
}

//========================================================================================================================
bool FEOtsUDPFSSRInterface::running(void)
{
	std::cout << __COUT_HDR_FP__ << std::endl;
	__COUT__ << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << std::endl;
	//usleep(1000000);
	std::string csrRegisterRead;
	std::string csrRegisterBuffer = FSSRFirmware::readCSRRegister();
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), csrRegisterRead);
	uint32_t csrRegisterValue = FSSRFirmware::createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware::setCSRRegister(csrRegisterValue);
	__COUT__ << "0STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	__COUT__ << "0STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	__COUT__ << "0STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	__COUT__ << "0STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	__COUT__ << "0STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	usleep(1000000);
	csrRegisterRead.clear();
	csrRegisterBuffer.resize(8,0);
	OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), csrRegisterRead);
	csrRegisterValue = FSSRFirmware::createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware::setCSRRegister(csrRegisterValue);
	__COUT__ << "1STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
	__COUT__ << "1STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;

	if(csrRegisterValue&0x00080000)
	{
		__COUT__ << "SLEEPING!!!! STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
		__COUT__ << "STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
		usleep(500000);

		csrRegisterRead.clear();
		csrRegisterBuffer.resize(8,0);
		OtsUDPHardware::read(FSSRFirmware::readCSRRegister(), csrRegisterRead);
		csrRegisterValue = FSSRFirmware::createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		FSSRFirmware::setCSRRegister(csrRegisterValue);
		__COUT__ << "1STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;
		__COUT__ << "1STRIP CSR Register value:" << std::hex << csrRegisterValue << std::dec << std::endl;

		if(csrRegisterValue&0x00080000)
		{
			__SS__ << "Problem with CSR Register!" << __E__;
			__SS_THROW__;
		}
		return true;
	}


	std::string sccsrRegisterRead;
	std::string sccsrRegisterBuffer;
	sccsrRegisterBuffer = FSSRFirmware::readSCCSRRegister();

	OtsUDPHardware::read(FSSRFirmware::readSCCSRRegister(), sccsrRegisterRead);
	uint32_t sccsrRegisterValue = FSSRFirmware::createRegisterFromValue(sccsrRegisterBuffer, sccsrRegisterRead);
	__COUT__ << "1STRIP SC CSR Register value:" << std::hex << sccsrRegisterValue << std::dec << std::endl;

//    OtsUDPHardware::write(FSSRFirmware::resetBCO());
//
//	sccsrRegisterBuffer.clear();
//	sccsrRegisterBuffer = FSSRFirmware::readSCCSRRegister();
//	OtsUDPHardware::read(FSSRFirmware::readSCCSRRegister(), sccsrRegisterRead);
//	sccsrRegisterValue = FSSRFirmware::createRegisterFromValue(sccsrRegisterBuffer, sccsrRegisterRead);
//	__COUT__ << "2STRIP SC CSR Register value:" << std::hex << sccsrRegisterValue << std::dec << std::endl;

	OtsUDPHardware::write(FSSRFirmware::resetBCO());
	std::string buffer;
	OtsUDPFirmwareCore::startBurst(buffer);
	OtsUDPHardware::write(buffer);
	return false;
}

//========================================================================================================================
void FEOtsUDPFSSRInterface::stop(void)
{
	__COUT__ << "\tStop" << std::endl;
	OtsUDPHardware::write(FSSRFirmware::stopStream());
	std::string buffer;
	OtsUDPFirmwareCore::stopBurst(buffer);
	OtsUDPHardware::write(buffer);
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for returnValue must be max UDP size to handle return possibility
int ots::FEOtsUDPFSSRInterface::universalRead(char *address, char *returnValue)
{
    __COUT__ << "address size " << universalAddressSize_ << std::endl;

    __COUT__ << "Request: ";
	for(unsigned int i=0;i<universalAddressSize_;++i)
		printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

	std::string readBuffer(universalDataSize_,0); //0 fill to correct number of bytes
    OtsUDPHardware::read(FSSRFirmware::universalRead(address), readBuffer); // data reply

    __COUT__ << "Result SIZE: " << readBuffer.size() << std::endl;
    memcpy(returnValue,readBuffer.substr(2).c_str(),universalDataSize_);
    return 0;
}

//========================================================================================================================
//NOTE: buffer for address must be at least size universalAddressSize_
//NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::FEOtsUDPFSSRInterface::universalWrite(char* address, char* writeValue)
{

    __COUT__ << "address size " << universalAddressSize_ << std::endl;
    __COUT__ << "data size " << universalDataSize_ << std::endl;
    __COUT__ << "Sending: ";
    for(unsigned int i=0;i<universalAddressSize_;++i)
    	printf("%2.2X",(unsigned char)address[i]);
	std::cout << std::endl;

    OtsUDPHardware::write(FSSRFirmware::universalWrite(address,writeValue)); // data request
}

DEFINE_OTS_INTERFACE(FEOtsUDPFSSRInterface)
