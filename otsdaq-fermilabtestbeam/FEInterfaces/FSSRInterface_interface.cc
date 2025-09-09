#include "otsdaq-fermilabtestbeam/FEInterfaces/FSSRInterface.h"

#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/Macros/InterfacePluginMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

#include "otsdaq-components/DAQHardware/FrontEndHardwareBase.h"
#include "otsdaq-components/DAQHardware/OtsUDPHardware.h"
#include "otsdaq-components/DAQHardware/OtsUDPFirmwareCore.h"
#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/FSSRFirmwareBase.h"
#include "otsdaq-fermilabtestbeam/DetectorConfiguration/DACStream.h"

#include <unistd.h>
#include <iostream>
#include <set>
#include <stdexcept>  // For std::exception
#include <thread>     // For std::this_thread::sleep_for
#include <chrono>   

using namespace ots;

//========================================================================================================================
FSSRInterface::FSSRInterface(const std::string &interfaceUID,
							 const ConfigurationTree &theXDAQContextConfigTree,
							 const std::string &interfaceConfigurationPath)
	: FEVInterface(interfaceUID, theXDAQContextConfigTree, interfaceConfigurationPath), FSSRFirmware_(nullptr), FSSRHardware_(nullptr), hardwareType_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
																																						  .getNode("HardwareType")
																																						  .getValue<std::string>()),
	  firmwareType_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
						.getNode("FirmwareType")
						.getValue<std::string>()),
	  firmwareVersion_(theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
						   .getNode("FirmwareVersion")
						   .getValue<unsigned int>())
{
	//__CFG_COUT__ use this instead of std::cout
	std::cout << "Constructing FSSRInterface. "
				 "=================================================================="
			  << std::endl;

	universalAddressSize_ = 8;
	universalDataSize_ = 8;

	// Setup firmware instance
	std::cout << "FirmwareType: " << firmwareType_ << std::endl;
	std::cout << "FirmwareVersion: " << firmwareVersion_ << std::endl;

	// choose: PurdueFSSRFirmware or OtsUDPFSSRFirmware (Right now we are running with the
	// OtsUDPFSSRFirmware)
	if (firmwareType_ == FSSRFirmwareBase::PURDUE_FIRMWARE_NAME)
		FSSRFirmware_ = new FSSRFirmwareBase(firmwareType_, firmwareVersion_);
	else if (firmwareType_ == FSSRFirmwareBase::OTS_FIRMWARE_NAME)
		FSSRFirmware_ = new FSSRFirmwareBase(firmwareType_, firmwareVersion_);
	else
	{
		__SS__ << "Unknown applicationFirmwareType type choice: " << firmwareType_
			   << std::endl;
		__CFG_COUT_ERR__ << ss.str();
		throw std::runtime_error(ss.str());
	}

	// Setup hardware instance
	std::cout << "HardwareType: " << hardwareType_ << std::endl;

	// choose: OtsUDPHardware or OtsUDPHardware ?? //FIXME? ? why does purdue use same
	// hardware? (this is how interface was ... why?)
	if (hardwareType_ == "PurdueHardware")
		FSSRHardware_ = new OtsUDPHardware(
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("HostIPAddress")
				.getValue<std::string>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("HostPort")
				.getValue<unsigned int>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("InterfaceIPAddress")
				.getValue<std::string>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("InterfacePort")
				.getValue<unsigned int>());
	else if (hardwareType_ == "OtsUDPHardware")
		FSSRHardware_ = new OtsUDPHardware(
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("HostIPAddress")
				.getValue<std::string>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("HostPort")
				.getValue<unsigned int>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("InterfaceIPAddress")
				.getValue<std::string>(),
			theXDAQContextConfigTree.getNode(interfaceConfigurationPath)
				.getNode("InterfacePort")
				.getValue<unsigned int>());
	else
	{
		__SS__ << "Unknown hardware type choice: " << hardwareType_ << std::endl;
		__CFG_COUT_ERR__ << ss.str();
		throw std::runtime_error(ss.str());
	}

	std::cout << "Constructor complete. "
				 "=================================================================="
			  << std::endl;
}

//========================================================================================================================
FSSRInterface::~FSSRInterface(void)
{
	delete FSSRFirmware_;
	FSSRFirmware_ = nullptr;
	delete FSSRHardware_;
	FSSRHardware_ = nullptr;
}

//========================================================================================================================
void FSSRInterface::configure(void)
{
	std::cout
		<< "============================================================================"
		<< std::endl;
	std::cout
		<< "Configure =================================================================="
		<< std::endl;
	std::string writeBuffer;
	std::string readBuffer;
	uint64_t tmp;

	stop();
	if (firmwareType_ == FSSRFirmwareBase::OTS_FIRMWARE_NAME)
	{
		std::cout << "Resetting Ethernet!" << std::endl;
		((OtsUDPFirmwareCore *)(FSSRFirmware_->communicationFirmwareInstance_))
			->softEthernetReset(writeBuffer);
		FSSRHardware_->write(writeBuffer);
		((OtsUDPFirmwareCore *)(FSSRFirmware_->communicationFirmwareInstance_))
			->clearEthernetReset(writeBuffer);
		FSSRHardware_->write(writeBuffer);
		// std::cout  << "Sleeping 1 second..." << std::endl;
		//  sleep(1); //seconds
	}

	std::cout << "Clearing receive socket buffer: " << FSSRHardware_->flushRead()
			  << " packets cleared." << std::endl;

	std::string streamToIP = theXDAQContextConfigTree_.getNode(theConfigurationPath_)
								 .getNode("StreamToIPAddress")
								 .getValue<std::string>();
	unsigned int streamToPort = theXDAQContextConfigTree_.getNode(theConfigurationPath_)
									.getNode("StreamToPort")
									.getValue<unsigned int>();

	std::cout << "Setting destination IP:   " << streamToIP << std::endl;
	std::cout << "Setting destination port: " << streamToPort << std::endl;

	FSSRFirmware_->communicationFirmwareInstance_->setDataDestination(
		writeBuffer, streamToIP, streamToPort);
	FSSRHardware_->write(writeBuffer);
	// std::cout << "Data destination set!" << std::endl;

	/*
	try
	{
		if (firmwareType_ == FSSRFirmwareBase::OTS_FIRMWARE_NAME)
		{

			std::cout << "Reading back burst dest MAC/IP/Port: "  << std::endl;

			((OtsUDPFirmwareCore*)(FSSRFirmware_->communicationFirmwareInstance_))->readDataDestinationMAC(writeBuffer);
			FSSRHardware_->read(writeBuffer, readBuffer);
			std::cout << "Destination MAC Address: ";
			for (uint32_t i = 0; i < readBuffer.size(); i++)
				printf("%2.2X-", (((int16_t)readBuffer[i]) & 0xFF));
			std::cout << std::endl;

			((OtsUDPFirmwareCore*)(FSSRFirmware_->communicationFirmwareInstance_))->readDataDestinationIP(writeBuffer);
			FSSRHardware_->read(writeBuffer, readBuffer);
			std::cout << "Destination IP: ";
			for (uint32_t i = 0; i < readBuffer.size(); i++)
				printf("%2.2X-", (((int16_t)readBuffer[i]) & 0xFF));
			std::cout << std::endl;

			((OtsUDPFirmwareCore*)(FSSRFirmware_->communicationFirmwareInstance_))->readDataDestinationPort(writeBuffer);
			FSSRHardware_->read(writeBuffer, readBuffer);
			std::cout << "Destination Port: ";
			for (uint32_t i = 0; i < readBuffer.size(); i++)
				printf("%2.2X-", (((int16_t)readBuffer[i]) & 0xFF));
			std::cout << std::endl;
		}
	}
	catch (...)
	{
		std::cout << "Error reading while configuring." << std::endl;
		throw;
	}
*/
	std::cout << "Done configuring Ethernet block." << std::endl;

	std::string value;
	std::string CSRRegister = FSSRFirmware_->readCSRRegister();

	FSSRHardware_->read(CSRRegister, value, 2);
	uint32_t registerValue = FSSRFirmware_->createRegisterFromValue(CSRRegister, value);
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << " -> Initial STRIP CSR Register value: 0x" << std::hex
			  << registerValue << std::dec << std::endl;

	FSSRFirmware_->setCSRRegister(0); // registerValue);//WHY 0?????????

	//	return;

	/////////////////////////////////////////////////
	std::cout << "Configuring clocks..." << std::endl;
	std::cout << "Clock source:    "
			  << theXDAQContextConfigTree_.getNode(theConfigurationPath_)
					 .getNode("ClockSelect")
					 .getValue<std::string>()
			  << std::endl;
	std::cout << "Clock frequency: "
			  << theXDAQContextConfigTree_.getNode(theConfigurationPath_)
					 .getNode("ClockSpeedMHz")
					 .getValue<float>()
			  << " MHz" << std::endl;
	FSSRHardware_->write(FSSRFirmware_->configureClocks(
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ClockSelect")
			.getValue<std::string>(),
		//"Internal",
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ClockSpeedMHz")
			.getValue<float>()));

	// FSSRHardware_->writeAndAcknowledge(FSSRFirmware_->resetDetector(),10);

	//
	//	std::cout << std::endl;
	//
	usleep(200000);
	// LORE 2018/10/22 Commented out because it is done inside the configure clocks twice!
	// writeBuffer.resize(0);
	// FSSRFirmware_->resetDCM(writeBuffer);
	// FSSRHardware_->write(writeBuffer);
	// usleep(100000);

	writeBuffer.resize(0);
	//////////////////////////////////////////////////////////////////
	// ORIGINAL TILL 2018/05/2 -> the explanation on how to align the readout is inside
	// the method  FSSRFirmware_->alignReadOut(writeBuffer, 0x1e);//SEEMED TO BE USELESS
	// for(unsigned int i=0; i<numberOfTicks; i++)
	//{
	// FSSRFirmware_->alignReadOut(writeBuffer, sensor, chip);
	auto feDetectorList = theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
							  .getNode("LinkToFEToDetectorTable")
							  .getChildren();

	uint8_t channelsAlignment[6] = {0, 0, 0, 0, 0, 0};
	for (auto &it : feDetectorList)
	{
		unsigned int feChannel = it.second.getNode("FEChannel").getValue<unsigned int>();
		const unsigned int rocBaseAddress = 9;
		unsigned int rocAddress = it.second.getNode("ROCAddress").getValue<unsigned int>();
		unsigned int rocAlign = it.second.getNode("ROCAlign").getValue<unsigned int>();
		if (feChannel > 5)
		{
			__SS__ << "Invalid FEChannel parameter value in FEToDetectorTable " << feChannel << ". Values can only be between 0 and 5." << std::endl;
			__COUT_ERR__ << "\n"
						 << ss.str();
			__SS_THROW__;
		}
		channelsAlignment[feChannel] += (rocAlign << (rocAddress - rocBaseAddress));
	}
	std::cout << std::hex << "ALIGNEMENT: " << channelsAlignment[0] << ":" << channelsAlignment[1] << std::dec << std::endl;
	FSSRFirmware_->alignReadOut(writeBuffer, channelsAlignment[0], channelsAlignment[1], channelsAlignment[2], channelsAlignment[3], channelsAlignment[4], channelsAlignment[5]);
	FSSRHardware_->write(writeBuffer);

	FSSRHardware_->read(FSSRFirmware_->readTrimCSRRegister(), value, 2);
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable") << " -> STRIP TRIM CSR Register value: 0x" << std::hex << value << std::dec << std::endl;
	//}
	//////////////////////////////////////////////////////////////////
	writeBuffer.resize(0);
	FSSRHardware_->write(FSSRFirmware_->resetDetector());
	usleep(50000); // Need sometime to clear

	// FSSRHardware_->read(CSRRegister, value);
	// registerValue = FSSRFirmware_->createRegisterFromValue(CSRRegister, value);
	// std::cout << "STRIP CSR Register value: 0x" << std::hex << registerValue <<
	// std::dec << std::endl;

	configureDetector();

	FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), value, 2);
	CSRRegister = FSSRFirmware_->readCSRRegister();
	registerValue = FSSRFirmware_->createRegisterFromValue(CSRRegister, value);
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << " -> STRIP CSR Register value: 0x" << std::hex << registerValue
			  << std::dec << std::endl;
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << " -> STRIP CSR Register value: 0x" << std::hex << registerValue
			  << std::dec << std::endl;
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << " -> STRIP CSR Register value: 0x" << std::hex << registerValue
			  << std::dec << std::endl;
	std::cout << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << " -> STRIP CSR Register value: 0x" << std::hex << registerValue
			  << std::dec << std::endl;
	std::cout << "Configure done "
				 "================================================================"
			  << std::endl;
}

//========================================================================================================================
void FSSRInterface::configureDetector(void)
{
	std::cout << "Configuring Detector: "
			  << theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
					 .getNode("LinkToFEToDetectorTable")
			  << "  =========================================" << std::endl;
	// std::cout <<
	//	for(const auto& interface:
	//  theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable").getChildren())
	//	{
	//		std::cout << interface.first << std::endl;
	//		std::cout <<
	//  interface.second.getNode("FEWriterDetectorAddress").getValue<unsigned int>() <<
	//  std::endl;
	//	}

	// std::cout << std::endl;

	DACStream theDACStream;
	theDACStream.makeStream(theXDAQContextConfigTree_.getBackNode(theConfigurationPath_)
								.getNode("LinkToFEToDetectorTable"));
	// std::cout << std::endl;
	//  NOTE This way we first upload all registers to the FPGA and then all together to
	//  the ROCS
	std::set<unsigned int> fecChannel;
	bool anyOn = false;

	// std::cout << std::endl;
	for (DACStream::const_iterator it = theDACStream.getChannelStreamMap().begin();
		 it != theDACStream.getChannelStreamMap().end();
		 it++)
	{
		// std::cout << std::endl;
		if (it->second.getROCStatus())
		{
			anyOn = true;
			// FIXME This is not an hardware task since the hardware can be used by FEW
			// and FER  theHardware_.uploadDACsToFEW(it->first,it->second);//TODO Maybe
			// add in the firmware the DAC version so there is no need to copy the DACs
			// again std::vector<std::string> sendBuffer; std::vector<std::string>
			// receiveBuffer;
			std::vector<std::string> sendBuffer;
			// std::string sendBuffer;
			// std::string receiveBuffer;
			// string buffer;
			FSSRFirmware_->makeDACBuffer(sendBuffer, it->first, it->second);

			std::cout << "Configuring channel: " << it->first << std::endl;
			FSSRHardware_->write(sendBuffer);
			// std::cout << std::endl;
			usleep(100000);

			//			FSSRHardware_->read(sendBuffer,receiveBuffer);
			//			std::vector<std::string>::iterator sendIt    = sendBuffer.begin();
			//			std::vector<std::string>::iterator receiveIt =
			// receiveBuffer.begin(); 			for(; sendIt!=sendBuffer.end(); sendIt++,
			// receiveIt++)
			//			{
			//				std::string toFix =
			// FSSRFirmware_->compareSendAndReceive(*sendIt, *receiveIt);
			// if(!toFix.empty())
			//				{
			//					std::string receivedFixed;
			//					FSSRHardware_->read(toFix,receivedFixed);
			//					std::string toFixAgain =
			// FSSRFirmware_->compareSendAndReceive(toFix, receivedFixed);
			//					if(!toFixAgain.empty())
			//					{
			//						std::cout << "ERROR: I tried to re-send " <<
			// std::endl;
			//
			//						for (unsigned int i = 0; i < toFixAgain.size(); i++)
			//							std::cout << (unsigned int)toFixAgain[i] <<
			// std::endl;
			//
			//						std::cout << " and got an error again!" <<
			// std::endl;
			//					}
			//				}
			//			}

			std::string maskBuffer;
			// std::cout << std::endl;
			FSSRFirmware_->makeMaskBuffer(maskBuffer, it->first, it->second);
			usleep(100000);

			// std::cout << std::endl;
			//			FSSRHardware_->writeAndAcknowledge(maskBuffer);
			FSSRHardware_->write(maskBuffer);
			// std::cout << std::endl;

			fecChannel.insert(it->first);
			// conbinedBuffer += buffer;
		}
	}

	// std::cout << std::endl;

	if (!anyOn)
		std::cout << "NOTE: No ROCs are on!" << std::endl;
	// FIXME This is not an hardware task since the hardware can be used by FEW and FER
	//    for(set<unsigned int>::const_iterator it=fecChannel.begin();
	//    it!=fecChannel.end(); it++)
	// uploadDACsToDetector(*it);

	std::cout
		<< "Done Configuring Detector   ============================================="
		<< std::endl;
}

//========================================================================================================================
void FSSRInterface::halt(void)
{
	std::cout << "\tHalt" << std::endl;
	stop();
}

//========================================================================================================================
void FSSRInterface::pause(void)
{
	std::cout << "\tPause" << std::endl;
	stop();
}

//========================================================================================================================
void FSSRInterface::resume(void)
{
	std::cout << "\tResume" << std::endl;
	start("");
}

//========================================================================================================================
/*
void FSSRInterface::start(std::string)  // runNumber)
{
	unsigned int i = VStateMachine::getIterationIndex();
	if(i == 0)
	{
		VStateMachine::indicateIterationWork();
		std::cout << "\tStart" << std::endl;
		//std::cout << "FE Detector config link = " <<
		// theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable")
		//<< std::endl;

		std::string csrRegisterBuffer;
		std::string csrRegisterRead;
		uint32_t    csrRegisterValue;

		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		if(csrRegisterValue == 0)
			std::cout << "\tERROR - Problem with register! See above" << std::endl;
		FSSRFirmware_->setCSRRegister(csrRegisterValue);

		FSSRHardware_->write(FSSRFirmware_->enableTrigger());
		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;

		//	usleep(50000);
		// NOW
		// FSSRHardware_->write(FSSRFirmware_->resetBCO());
		// usleep(50000);

		FSSRHardware_->write(FSSRFirmware_->armBCOReset());
		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 2 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 2 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 2 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		// std::string buffer;
		////LORE THIS ONE NEEDED BY RYAN OTS FSSR
		//((OtsUDPFirmwareCore*)FSSRFirmware_)->startBurst(buffer);
		// FSSRHardware_->write(buffer);

		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;

		// NOW
		FSSRHardware_->write(FSSRFirmware_->startStream(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus0")
				.getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus1")
				.getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus2")
				.getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus3")
				.getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus4")
				.getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_)
				.getNode("ChannelStatus5")
				.getValue<bool>()));

		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 4 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 4 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 4 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;

		{
			std::string buffer;
			// LORE THIS ONE NEEDED BY RYAN OTS FSSR
			((OtsUDPFirmwareCore*)FSSRFirmware_)->startBurst(buffer);
			FSSRHardware_->write(buffer);
		}
		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 5 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 5 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> START 5 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
	}
	else if(i == 1)
	{
		//	return false;
		std::string csrRegisterBuffer;
		std::string csrRegisterRead;
		uint32_t    csrRegisterValue;

		csrRegisterBuffer = FSSRFirmware_->readCSRRegister();

		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		FSSRFirmware_->setCSRRegister(csrRegisterValue);

		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   0 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		usleep(100000);

		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		FSSRFirmware_->setCSRRegister(csrRegisterValue);
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUN   1 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;

		if(csrRegisterValue & 0x00080000)
		{
			csrRegisterRead.clear();
			FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
			csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer,
																	  csrRegisterRead);
			FSSRFirmware_->setCSRRegister(csrRegisterValue);
			std::cout << FEVInterface::interfaceUID_
						 << " -> RUN   2 STRIP CSR Register value: 0x" << std::hex
						 << csrRegisterValue << std::dec << std::endl;
			std::cout << FEVInterface::interfaceUID_
						 << " -> RUN   2 STRIP CSR Register value: 0x" << std::hex
						 << csrRegisterValue << std::dec << std::endl;
			std::cout << FEVInterface::interfaceUID_
						 << " -> RUN   2 STRIP CSR Register value: 0x" << std::hex
						 << csrRegisterValue << std::dec << std::endl;
			usleep(1000000);
			if(VStateMachine::getSubIterationIndex() < 5)
			{
				VStateMachine::indicateSubIterationWork();
				return;
			}
		}

		csrRegisterRead.clear();
		FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
		csrRegisterValue =
			FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
		FSSRFirmware_->setCSRRegister(csrRegisterValue);
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUNNING 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUNNING 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;
		std::cout << FEVInterface::interfaceUID_
					 << " -> RUNNING 3 STRIP CSR Register value: 0x" << std::hex
					 << csrRegisterValue << std::dec << std::endl;

		FSSRHardware_->write(FSSRFirmware_->resetBCO());
		//	std::string buffer;

		// FSSRHardware_->write(FSSRFirmware_->startStream(
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus0").getValue<bool>(),
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus1").getValue<bool>(),
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus2").getValue<bool>(),
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus3").getValue<bool>(),
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus4").getValue<bool>(),
		//         theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus5").getValue<bool>()
		// ));

		// LORE THIS ONE NEEDED BY RYAN OTS FSSR
		//	((OtsUDPFirmwareCore*)FSSRFirmware_)->startBurst(buffer);
		//	FSSRHardware_->write(buffer);
	}
}
*/
//========================================================================================================================
void FSSRInterface::start(std::string) // runNumber)
{
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << "\tStart" << std::endl;
	// std::cout << "FE Detector config link = " <<
	//  theXDAQContextConfigTree_.getBackNode(theConfigurationPath_).getNode("LinkToFEToDetectorTable")
	//<< std::endl;

	std::string csrRegisterBuffer;
	std::string csrRegisterRead;
	uint32_t csrRegisterValue;
	
	//Added to see if we get what we expect from the register
	
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	
	std::cout << "Writing armBCOReset " << std::endl;
	FSSRHardware_->write(FSSRFirmware_->armBCOReset());
	
	std::cout << "Writing enableTrigger " << std::endl;
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	FSSRHardware_->write(FSSRFirmware_->enableTrigger());
	
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	
	auto [successStart1, errorMsgStart1] = tryReadWithRetries(csrRegisterRead);
	if (!successStart1)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart1 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}


	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
			  << FEVInterface::interfaceUID_
			  << " -> START 0 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	if (csrRegisterValue == 0)
		std::cout << "\tERROR - Problem with register! See above" << std::endl;
	FSSRFirmware_->setCSRRegister(csrRegisterValue);

	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	FSSRHardware_->write(FSSRFirmware_->enableTrigger());

	//csrRegisterRead.clear();
	
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successStart2, errorMsgStart2] = tryReadWithRetries(csrRegisterRead);
	if (!successStart2)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart2 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	
	
	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
			  << FEVInterface::interfaceUID_
			  << " -> START 1 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;

	//	usleep(50000);
	// NOW
	// FSSRHardware_->write(FSSRFirmware_->resetBCO());
	// usleep(50000);

	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	FSSRHardware_->write(FSSRFirmware_->armBCOReset());

	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successStart3, errorMsgStart3] = tryReadWithRetries(csrRegisterRead);
	if (!successStart3)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart3 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}

	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ 
	          << FEVInterface::interfaceUID_
			  << " -> START 2 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	// std::string buffer;
	////LORE THIS ONE NEEDED BY RYAN OTS FSSR
	//((OtsUDPFirmwareCore*)FSSRFirmware_)->startBurst(buffer);
	// FSSRHardware_->write(buffer);



	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	
	auto [successStart4, errorMsgStart4] = tryReadWithRetries(csrRegisterRead);
	if (!successStart4)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart4 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	<< FEVInterface::interfaceUID_
	<< " -> START 3 STRIP CSR Register value: 0x" << std::hex
	<< csrRegisterValue << std::dec << std::endl;

	// NOW
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	FSSRHardware_->write(FSSRFirmware_->startStream(
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus0")
			.getValue<bool>(),
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus1")
			.getValue<bool>(),
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus2")
			.getValue<bool>(),
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus3")
			.getValue<bool>(),
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus4")
			.getValue<bool>(),
		theXDAQContextConfigTree_.getNode(theConfigurationPath_)
			.getNode("ChannelStatus5")
			.getValue<bool>()));

	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__ << std::endl;
	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	
	auto [successStart5, errorMsgStart5] = tryReadWithRetries(csrRegisterRead);
	if (!successStart5)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart5 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
			  << FEVInterface::interfaceUID_
			  << " -> START 4 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;


	// This is needed to get some data as of now September 4th, 2025. 
	// {
	// 	std::string buffer;
	// 	// LORE THIS ONE NEEDED BY RYAN OTS FSSR
	// 	((OtsUDPFirmwareCore *)FSSRFirmware_)->startBurst(buffer);
	// 	FSSRHardware_->write(buffer);
	// }
	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successStart6, errorMsgStart6] = tryReadWithRetries(csrRegisterRead);
	if (!successStart6)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStart6 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	
	csrRegisterValue = FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	    	  << FEVInterface::interfaceUID_
			  << " -> START 5 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
}

//========================================================================================================================
bool FSSRInterface::running(void)
{
	//return false;
	std::string csrRegisterBuffer;
	std::string csrRegisterRead;
	uint32_t csrRegisterValue;

	// Just to see if it can wait this time before the triggers start and it can get the register down
	usleep(1000000);

	csrRegisterBuffer = FSSRFirmware_->readCSRRegister();

	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	
	auto [successRun1, errorMsgRun1] = tryReadWithRetries(csrRegisterRead);
	if (!successRun1)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgRun1 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);

	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	          << FEVInterface::interfaceUID_
			  << " -> RUN   0 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	//usleep(100000);

	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	
	auto [successRun2, errorMsgRun2] = tryReadWithRetries(csrRegisterRead);
	if (!successRun2)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgRun2 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	          << FEVInterface::interfaceUID_
			  << " -> RUN   1 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;

	// if (csrRegisterValue & 0x00080000)
	// {
	// 	csrRegisterRead.clear();
	// 	FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead);
	// 	csrRegisterValue =
	// 		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	// 	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	// 	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	// 	          << FEVInterface::interfaceUID_
	// 			  << " -> RUN   2 STRIP CSR Register value: 0x" << std::hex
	// 			  << csrRegisterValue << std::dec << std::endl;
	// 	usleep(1000000);
	// 	//return true;
	// }

	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successRun3, errorMsgRun3] = tryReadWithRetries(csrRegisterRead);
	if (!successRun3)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgRun3 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	          << FEVInterface::interfaceUID_
			  << " -> RUN   2 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
		//return true;


	csrRegisterRead.clear();
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successRun4, errorMsgRun4] = tryReadWithRetries(csrRegisterRead);
	if (!successRun4)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgRun4 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	std::cout << "[" << __LINE__ << "] " << __PRETTY_FUNCTION__
	          << FEVInterface::interfaceUID_
			  << " -> RUN 3 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;

	// FSSRHardware_->write(FSSRFirmware_->resetBCO());
	//	std::string buffer;
	/*
	FSSRHardware_->write(FSSRFirmware_->startStream(
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus0").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus1").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus2").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus3").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus4").getValue<bool>(),
			theXDAQContextConfigTree_.getNode(theConfigurationPath_).getNode("ChannelStatus5").getValue<bool>()
	));
	 */
	//LORE THIS ONE NEEDED BY RYAN OTS FSSR
	// std::string buffer;
	// ((OtsUDPFirmwareCore*)FSSRFirmware_)->startBurst(buffer);
	// FSSRHardware_->write(buffer);
	return false;
}

//========================================================================================================================
void FSSRInterface::stop(void)
{
	std::cout << "\tStop" << std::endl;
	std::string csrRegisterBuffer;
	std::string csrRegisterRead;
	uint32_t csrRegisterValue;

	csrRegisterRead.clear();
	FSSRHardware_->write(FSSRFirmware_->stopStream());
	std::cout << "I was able to write a stopStream before reading " << std::endl;

	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successStop, errorMsgStop] = tryReadWithRetries(csrRegisterRead);
	if (!successStop)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStop << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}
	
	
	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 0 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 0 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 0 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;

	FSSRHardware_->write(FSSRFirmware_->stopStream());
	std::cout << "I was able to write a 2nd stopStream before reading " << std::endl;
	//FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), csrRegisterRead, 2);
	auto [successStop2, errorMsgStop2] = tryReadWithRetries(csrRegisterRead);
	if (!successStop2)
	{std::cerr << "❌ Failed to read CSR register after retries. Last error: "
				<< errorMsgStop2 << std::endl;}
	else
	{std::cout << "✅ Successfully read CSR register." << std::endl;	}

	csrRegisterValue =
		FSSRFirmware_->createRegisterFromValue(csrRegisterBuffer, csrRegisterRead);
	FSSRFirmware_->setCSRRegister(csrRegisterValue);
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 1 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 1 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	std::cout << FEVInterface::interfaceUID_
			  << " -> STOP 1 STRIP CSR Register value: 0x" << std::hex
			  << csrRegisterValue << std::dec << std::endl;
	std::string buffer;
	// LORE THIS ONE NEEDED BY RYAN OTS FSSR
	((OtsUDPFirmwareCore *)FSSRFirmware_)->stopBurst(buffer);
	FSSRHardware_->write(buffer);
}

//========================================================================================================================
// NOTE: buffer for address must be at least size universalAddressSize_
// NOTE: buffer for returnValue must be max UDP size to handle return possibility
void ots::FSSRInterface::universalRead(char *address, char *returnValue)
{
	std::cout << "address size " << universalAddressSize_ << std::endl;

	std::cout << "Request: ";
	for (unsigned int i = 0; i < universalAddressSize_; ++i)
		printf("%2.2X", (unsigned char)address[i]);
	std::cout << std::endl;

	std::string readBuffer(universalDataSize_, 0);							// 0 fill to correct number of bytes
	FSSRHardware_->read(FSSRFirmware_->universalRead(address), readBuffer, 2); // data reply

	std::cout << "Result SIZE: " << readBuffer.size() << std::endl;
	memcpy(returnValue, readBuffer.substr(2).c_str(), universalDataSize_);
}

//========================================================================================================================
// NOTE: buffer for address must be at least size universalAddressSize_
// NOTE: buffer for writeValue must be at least size universalDataSize_
void ots::FSSRInterface::universalWrite(char *address, char *writeValue)
{
	std::cout << "address size " << universalAddressSize_ << std::endl;
	std::cout << "data size " << universalDataSize_ << std::endl;
	std::cout << "Sending: ";
	for (unsigned int i = 0; i < universalAddressSize_; ++i)
		printf("%2.2X", (unsigned char)address[i]);
	std::cout << std::endl;

	FSSRHardware_->write(
		FSSRFirmware_->universalWrite(address, writeValue)); // data request
}

//New method, can be erased if not needed


std::pair<bool, std::string> FSSRInterface::tryReadWithRetries(std::string &readBuffer)
{
    const int maxAttempts = 5;
    std::string lastError;

    for (int attempt = 0; attempt < maxAttempts; ++attempt)
    {
        try
        {
            // Attempt the hardware read
            FSSRHardware_->read(FSSRFirmware_->readCSRRegister(), readBuffer, 2);
            return {true, ""}; // Success!
        }
        catch (const std::exception &ex)
        {
            lastError = ex.what();
            std::cerr << "[WARNING] Attempt " << attempt + 1
                      << " failed: " << lastError << std::endl;

            if (attempt == maxAttempts - 1)
            {
                std::cerr << "[ERROR] All " << maxAttempts << " attempts failed." << std::endl;
                return {false, lastError};
            }
        }
        catch (...)
        {
            lastError = "Unknown exception";
            std::cerr << "[WARNING] Attempt " << attempt + 1
                      << " failed: " << lastError << std::endl;

            if (attempt == maxAttempts - 1)
            {
                std::cerr << "[ERROR] All " << maxAttempts << " attempts failed." << std::endl;
                return {false, lastError};
            }
        }
    }

    return {false, "Unexpected failure"};
}


DEFINE_OTS_INTERFACE(FSSRInterface)
