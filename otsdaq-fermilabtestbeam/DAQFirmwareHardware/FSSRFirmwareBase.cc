#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/FSSRFirmwareBase.h"
#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/FSSRFirmwareDefinitions.h"
#include "otsdaq-components/DAQHardware/FrontEndFirmwareBase.h"
#include "otsdaq-components/DAQHardware/OtsUDPFirmwareCore.h"
#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/PurdueFirmwareCore.h"

#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"

#include "otsdaq-fermilabtestbeam/DetectorHardware/FSSRROCDefinitions.h"
#include "otsdaq/BitManipulator/BitManipulator.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdlib>
#include <iostream>


using namespace ots;

const std::string FSSRFirmwareBase::PURDUE_FIRMWARE_NAME = "PurdueFSSRFirmware";
const std::string FSSRFirmwareBase::OTS_FIRMWARE_NAME    = "OtsUDPFSSRFirmware";

//==============================================================================
FSSRFirmwareBase::FSSRFirmwareBase(const std::string& communicationFirmwareType,
                                   unsigned int       communicationFirmwareVersion,
                                   unsigned int       version)
    : stripCSRRegisterValue_(0), communicationFirmwareType_(communicationFirmwareType)
{
	// choose: OtsUDPFirmwareCore or PurdueFirmwareCore
	if(communicationFirmwareType ==
	   FSSRFirmwareBase::PURDUE_FIRMWARE_NAME)  // could also use string and: if(choice ==
	                                            // "purdue") etc.
		communicationFirmwareInstance_ =
		    new PurdueFirmwareCore(communicationFirmwareVersion);
	else if(communicationFirmwareType ==
	        FSSRFirmwareBase::OTS_FIRMWARE_NAME)  // AUG-17-2017 RAR dissociated because
	                                              // function calls are entirely
	                                              // independent from PURDUE firmware
	                                              // calls
	                                              // //
		communicationFirmwareInstance_ =
		    new OtsUDPFirmwareCore(communicationFirmwareVersion);
	else
	{
		__SS__ << "Unknown communication firmware type choice: "
		       << communicationFirmwareType << std::endl;
		__COUT_ERR__ << ss.str();
		__SS_THROW__;
	}

	// now we can call write/read etc with
	//	communicationFirmwareInstance_->write,
	//	communicationFirmwareInstance_->read, etc
}

//==============================================================================
FSSRFirmwareBase::~FSSRFirmwareBase(void)
{
	delete communicationFirmwareInstance_;
	communicationFirmwareInstance_ = NULL;
}

//==============================================================================
void FSSRFirmwareBase::init(void) {}

//==============================================================================
std::string FSSRFirmwareBase::universalRead(char* address)
{
	__COUT__ << "universalRead communicationFirmwareType_ " << communicationFirmwareType_
	         << std::endl;
	return communicationFirmwareInstance_->read(address);
}

//==============================================================================
std::string FSSRFirmwareBase::universalWrite(char* address, char* data)
{
	__COUT__ << "universalWrite communicationFirmwareType_ " << communicationFirmwareType_
	         << std::endl;
	return communicationFirmwareInstance_->write(address, data);
}

//==============================================================================
uint32_t FSSRFirmwareBase::createRegisterFromValue(std::string& readBuffer,
                                                   std::string& receivedValue)
{
	return communicationFirmwareInstance_->createRegisterFromValue(readBuffer,
	                                                               receivedValue);
}

//==============================================================================
std::string FSSRFirmwareBase::configureClocks(std::string source, double frequency)
{
	// __COUT__ << "Writing Clock configuration!" << std::endl;

	std::string buffer;
	// NoNeedNowwrite(buffer, ETHIO_DESTINATION_PORT, 0x0000b798); //  Listen port for
	// ethio stuff

	setPacketSizeStripCSR(6);
	setExternalBCOClockSourceStripCSR(source);  //(source)
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_);  //  Reset CSR - reset trigger
	                                                 //  counter, external 27 MHz clock

	resetDCM(buffer);
	// alignReadOut(buffer,0x3000);//0x3000

	setFrequencyFromClockState(buffer, frequency);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);

	resetDCM(buffer);
	// __COUT__ << "stripCSRRegisterValue :" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl;

	return buffer;
}

//==============================================================================
void FSSRFirmwareBase::resetDCM(std::string& buffer)
{
	// 31    DCM_RESET
	resetDCMStripCSR(true);  // Set bit 31
	communicationFirmwareInstance_->write(buffer,
	                                      STRIP_CSR,
	                                      stripCSRRegisterValue_,
	                                      false /*clearBuffer*/);  //  Set reset to DCM

	resetDCMStripCSR(false);  // unset bit 31
	communicationFirmwareInstance_->write(buffer,
	                                      STRIP_CSR,
	                                      stripCSRRegisterValue_,
	                                      false /*clearBuffer*/);  //  Clear reset to DCM

	// 2018-10-24 Lorenzo in OtsUDPFirmware is doing nothing so I am commenting it out
	// communicationFirmwareInstance_->waitClear(buffer, STRIP_CSR,
	// waitDCMResetStripCSR(),false/*clearBuffer*/); //  Wait for DCM to lock
}

//==============================================================================
void FSSRFirmwareBase::alignReadOut(std::string& buffer, uint8_t channel0, uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4, uint8_t channel5)
{
	//(2:0) sensor
	//(7:3) chip
	//28 up/down (-> here we always move up)
	//31 must be set 1
	//It shifts the readout every time is called by few picosends (likely) if bit 31 is set

	//communicationFirmwareInstance_->write(buffer, STRIP_TRIM_CSR, (0x9<<28)+((chip&0xf)<<3)+(sensor&0x7)); //  MCLKB edge for channel 5 // was 0x00002000
	//communicationFirmwareInstance_->write(buffer, STRIP_TRIM_CSR, 0x3e0); //  MCLKB edge for channel 5 // was 0x00002000
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
	__COUT__ << "ALIGNING READOUT!!!!!" << std::endl;
//	communicationFirmwareInstance_->write(buffer, STRIP_TRIM_CSR, 0x0); //  MCLKB edge for channel 5 // was 0x00002000
	uint32_t registerValue =  ((channel5 & 0x1f)<<25) 
							+ ((channel4 & 0x1f)<<20) 
							+ ((channel3 & 0x1f)<<15) 
							+ ((channel2 & 0x1f)<<10) 
							+ ((channel1 & 0x1f)<<5) 
							+ ((channel0 & 0x1f)); 
	communicationFirmwareInstance_->write(buffer, STRIP_TRIM_CSR, registerValue); //  MCLKB edge for channel 5 // was 0x00002000
}
//==============================================================================
std::string FSSRFirmwareBase::resetDetector(int channel)
{
	// __COUT__ << "Resetting detector!" << std::endl;
	// Byte 4-------------------
	// 31    CHIP_RESET
	// 30    CLEAR_FIFO
	// 29    CLEAR_ERRORS
	// 28    LINK_RESET
	///////////////////////////
	// 27    DAC_RESET
	// 26 	unused
	// 25    unused
	// 24    unused
	// Byte 3-------------------
	// 23-16 unused
	// Byte 2-------------------
	// 15-8  unused
	// Byte 1-------------------
	// 7-0   CHANNEL_MASK ->Reset signals are sent to all channels with bits set in
	// CHANNEL_MASK field

	std::string buffer;
	if(channel == -1)  // reset all channels
	{
		// write(buffer,STRIP_RESET,0xd000003f);                  //  Issue reset
		communicationFirmwareInstance_->write(
		    buffer,
		    STRIP_RESET,
		    0xf000003f);  //  Issue reset // was 0xf000003f
		                  // 2018-10-24 Lorenzo in OtsUDPFirmware is doing nothing so I am
		                  // commenting it out
		                  // communicationFirmwareInstance_->waitClear(buffer,
		                  // STRIP_RESET, 0xf0000000); //  Wait for reset to complete //
		                  // was 0xf0000000
	}
	else
	{
		communicationFirmwareInstance_->write(
		    buffer,
		    STRIP_RESET,
		    0xf000003f);  //  Issue reset
		                  // 2018-10-24 Lorenzo in OtsUDPFirmware is doing nothing so I am
		                  // commenting it out
		                  // communicationFirmwareInstance_->waitClear(buffer,
		                  // STRIP_RESET, 0xf0000000); //  Wait for reset to complete
	}

	return buffer;
}

//==============================================================================
std::string FSSRFirmwareBase::enableTrigger(void)
{
	std::string buffer;
	// __COUT__ << "Enabling Trigger!!!" << std::endl;
	// __COUT__ << "stripCSRRegisterValue in :" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl;

	// setHaltStripCSR(1);//WARNING THIS IS CLEARED BY THE MASTER BUT IF THERE IS NO
	// MASTER NOTHING WORKS UNLESS THE BIT IS UNSET
	sendTriggerDataStripCSR(false);  // STRIP_CSR -> Bit 24 -> SEND_TRIG
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
	resetTriggerCounterStripCSR(buffer);
	sendTriggerDataStripCSR(true);    // STRIP_CSR -> Bit 24 -> SEND_TRIG
	sendTriggerNumberStripCSR(true);  // STRIP_CSR -> Bit 25 -> SEND_TRIGNUM
	sendBCOStripCSR(true);            // STRIP_CSR -> Bit 26 -> SEND_BCO
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);

	stripTriggerCSRRegisterValue_ = 0;
	BCOOffset(4);
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_TRIG_CSR,
	    stripTriggerCSRRegisterValue_,
	    false /*clearBuffer*/);  //  BCO offset // was 0x00000004

	//  write(buffer,STRIP_TRIG_INPUT_0,0x1f060040);  //  FSSR GOTHIT trigger input -
	//  timed in for the 27 MHz external clock
	//  write(buffer,STRIP_TRIG_INPUT_3,0x3f874040);  //  Unbiased trigger input +
	//  external trigger

	configureStripTriggerUnbiased(buffer);

	configureTriggerInputs(buffer);

	// FIXME for IP .36 the number to set is 0x20401000

	// if (1 || communicationFirmwareInstance_->getVersion() == 1)
	communicationFirmwareInstance_->write(
	    buffer, STRIP_TRIG_INPUT_3, 0x20401000, false /*clearBuffer*/);
	// else if (communicationFirmwareInstance_->getVersion() == 2)
	//	communicationFirmwareInstance_->write(buffer, STRIP_TRIG_INPUT_3, 0x20301000,
	// false/*clearBuffer*/);  else
	//{
	//	__SS__ << "what version is this?" << communicationFirmwareInstance_->getVersion()
	//<< std::endl;
	//	__COUT__ << ss.str();
	//	__SS_THROW__;
	//}
	// __COUT__ << "stripCSRRegisterValue out:" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl;  __COUT__ <<
	// "Done enabling Trigger!!!" << std::endl;

	return buffer;
}
//
//////==============================================================================
// void FSSRFirmwareBase::setDataDestination(std::string& buffer, const std::string& ip,
// const uint16_t port)
//////(std::string ip, uint32_t port)
//{
//    __COUT__ << "Set data destination!" << std::endl;
//
//    struct sockaddr_in socketAddress;
//    inet_pton(AF_INET, ip.c_str(), &(socketAddress.sin_addr));
//    __COUT__ << "ADDRESS: " << std::hex <<
//    ntohl(socketAddress.sin_addr.s_addr) << std::dec << std::endl;
//    communicationFirmwareInstance_->write(buffer, DATA_DESTINATION_IP,
//    ntohl(socketAddress.sin_addr.s_addr)); //  Set data destination IP 192.168.133.1
//    __COUT__ << "PORT: " << std::hex << port << std::dec <<
//    std::endl; communicationFirmwareInstance_->write(buffer,
//    DATA_SOURCE_DESTINATION_PORT, port); //  Set data destination port std::cout <<
//    __COUT_HDR_FL__ << "THIS IS THE BUFFER: " << buffer << std::endl;
//
//    for(uint32_t i=0; i<buffer.size(); i++)
//        printf("%2.2X-",(uint8_t)buffer[i]);
//    std::cout << std::dec << std::endl;
//
//}

//==============================================================================
std::string FSSRFirmwareBase::resetBCO(void)
{
	__COUT__ << "Reset BCO!!!" << std::endl;
	__COUT__ << "stripCSRRegisterValue in :" << std::hex << stripCSRRegisterValue_
	         << std::dec << std::endl;
	std::string buffer;

	// resetTriggerCounterStripCSR(buffer);
	// write(buffer, STRIP_CSR, stripCSRRegisterValue_);//the write is done in the reset
	// __COUT__ << "stripCSRRegisterValue :" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl;

	// msg->Write(STRIP_SC_CSR,0x90000b95|(chmask<<16));

	// Byte 4-------------------
	// 31    SC_BUSY
	// 30    RAW
	// 29    BCO_SYNC
	// 28    BCO_ZERO
	///////////////////////////
	// 27    unused
	// 26-24 LENGTH
	// Byte 3-------------------
	// 23-16 CHANNEL_MASK
	// Byte 2-------------------
	// 15-13 READ_SELECT
	// 12-10 INSTRUCTION
	// 9-5   ADDRESS
	// Byte 1-------------------
	// 9-5   ADDRESS
	// 4-0   CHIP_ID
	// 0x 9    0     3   f     0   b    9     5
	//  1001-0000-0011-1111-0000-1011-1001-0101
	// CHIP_ID      1-0101    -> 21 wild chip id ->all chips will get the command
	// ADDRESS      11-100    -> 28
	// INSTRUCTION  010       -> 2
	// READ_SELECT  000       -> 0
	// CHANNEL_MASK 0011-1111 -> all Channels
	// LENGTH       000       -> 0
	// unused
	// BCO_SYNC     1
	// BCO_ZERO     0
	// RAW          0
	// SC_BUSY      1
	// When BCO SYNC is set the transaction is delayed until the internal BCO counter has
	// a value of 240. This allow the AqBCO command to be synchronized
	communicationFirmwareInstance_->write(
	    buffer, STRIP_SC_CSR, 0x903f0b95, false /*clearBuffer*/);  //  This is the
	                                                               //  <SCR,set> command
	                                                               //  with the bit set to
	                                                               //  sync SHIFT with
	                                                               //  BCO=0.

	// enableBCOStripCSR(true);
	// communicationFirmwareInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);

	__COUT__ << "stripCSRRegisterValue out:" << std::hex << stripCSRRegisterValue_
	         << std::dec << std::endl;
	__COUT__ << "Done reset BCO!!!" << std::endl;

	return buffer;
}

//==============================================================================
std::string FSSRFirmwareBase::armBCOReset(void)
{
	std::string buffer;
	armBCOResetCSR();
	communicationFirmwareInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
	return buffer;
}

//==============================================================================
std::string FSSRFirmwareBase::startStream(bool channel0,
                                          bool channel1,
                                          bool channel2,
                                          bool channel3,
                                          bool channel4,
                                          bool channel5)
{
	//	__COUT__ << "Start Stream!" << std::endl;
	//	__COUT__ << "stripCSRRegisterValue in:" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl;
	std::string buffer;

	//	__COUT__ << " channel0 " << channel0 << " channel1 " <<
	// channel1 << " channel2 " << channel2 << " channel3 " << channel3 << " channel4 " <<
	// channel4 << " channel5 " << channel5 << std::endl;

	enableChannelsStripCSR(channel0, channel1, channel2, channel3, channel4, channel5);
	//    if (version_ == 1)
	//        enableChannelsStripCSR(true, true, true, true, false, false);
	//    else if (version_ == 2)
	//        enableChannelsStripCSR(true, true, true, true, true, true);

	enableStreamStripCSR(
	    true);  //  Turn on streaming hits along with BCO data // was 0x0f000f30
	communicationFirmwareInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);

	//	__COUT__ << "stripCSRRegisterValue out:" << std::hex <<
	// stripCSRRegisterValue_ << std::dec << std::endl; 	__COUT__
	// << "Done start Stream!" << std::endl;

	return buffer;
}

//==============================================================================
std::string FSSRFirmwareBase::stopStream(void)
{
	std::string buffer;
	enableStreamStripCSR(false);
	enableChannelsStripCSR(false, false, false, false, false, false);
	communicationFirmwareInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
	return buffer;
}

//==============================================================================
void FSSRFirmwareBase::makeDACSequence(FirmwareSequence<uint64_t>& sequence,
                                       unsigned int                channel,
                                       const ROCStream&            rocStream)
{
	const ROCDACs& rocDACs = rocStream.getROCDACs();
	for(DACList::const_iterator it = rocDACs.getDACList().begin();
	    it != rocDACs.getDACList().end();
	    it++)
	{
		// if(it->first != "SendData" && it->first != "RejectHits") continue;
		uint64_t data = FSSRROCDefinitions::makeDACWriteCommand(
		    rocStream.getFEWROCAddress(), it->first, it->second.second);
		sequence.pushBack(ChannelFIFOAddress[channel], data);
		sequence.pushBack(ChannelFIFOAddress[channel],
		                  BitManipulator::insertBits(data, (uint64_t)0x48, 56, 8));
		// set WRITE
		sequence.pushBack(ChannelFIFOAddress[channel],
		                  BitManipulator::insertBits(data, (uint64_t)1, 60, 1));
		// clr WRITE
		sequence.pushBack(ChannelFIFOAddress[channel],
		                  BitManipulator::insertBits(data, (uint64_t)0, 60, 1));
		// clr TALK
		sequence.pushBack(ChannelFIFOAddress[channel],
		                  BitManipulator::insertBits(data, (uint64_t)0, 62, 1));
		sequence.pushBack(ChannelFIFOAddress[channel],
		                  BitManipulator::insertBits(data, (uint64_t)0x40, 56, 8));
		// break;
	}
}

//==============================================================================
void FSSRFirmwareBase::makeDACSequence(FirmwareSequence<uint32_t>& sequence,
                                       unsigned int                channel,
                                       const ROCStream&            rocStream)
{
	const ROCDACs& rocDACs = rocStream.getROCDACs();
	for(DACList::const_iterator it = rocDACs.getDACList().begin();
	    it != rocDACs.getDACList().end();
	    it++)
	{
		/*RYAN
		 //if(it->first != "SendData" && it->first != "RejectHits") continue;
		 uint64_t data =
		 FSSRROCDefinitions::makeDACWriteCommand(rocStream.getFEWROCAddress(), it->first,
		 it->second.second); sequence.pushBack(ChannelFIFOAddress[channel], data);
		 sequence.pushBack(ChannelFIFOAddress[channel],
		 BitManipulator::insertBits(data,(uint32_t)0x48,56,8));
		 //set WRITE
		 sequence.pushBack(ChannelFIFOAddress[channel],
		 BitManipulator::insertBits(data,(uint32_t)1,60,1));
		 //clr WRITE
		 sequence.pushBack(ChannelFIFOAddress[channel],
		 BitManipulator::insertBits(data,(uint32_t)0,60,1));
		 //clr TALK
		 sequence.pushBack(ChannelFIFOAddress[channel],
		 BitManipulator::insertBits(data,(uint32_t)0,62,1));
		 sequence.pushBack(ChannelFIFOAddress[channel],
		 BitManipulator::insertBits(data,(uint32_t)0x40,56,8));
		 */

		// if(it->first != "SendData" && it->first != "RejectHits") continue;
		uint32_t data = FSSRROCDefinitions::makeDACWriteHeader(
		    rocStream.getFEWROCAddress(), it->first);
		// Insert channel
		BitManipulator::insertBits(data, 1, 16 + channel, 1);
		sequence.pushBack(ChannelFIFOAddress[channel], it->second.second);
		__COUT__ << "Register: " << it->first << " value: " << it->second.second
		         << std::hex << " -> Data: " << data << std::dec << std::endl;
		sequence.pushBack(STRIP_SC_CSR, data);
	}
}

//==============================================================================
void FSSRFirmwareBase::makeDACBuffer(std::string&     buffer,
                                     unsigned int     channel,
                                     const ROCStream& rocStream)
{
	__COUT__ << "Channel: " << channel << std::endl;
	__COUT__ << "BufferINsize: " << buffer.size() << std::endl;
	const ROCDACs& rocDACs = rocStream.getROCDACs();
	//	for (DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
	// rocDACs.getDACList().end(); it++)
	//	{
	//		std::string bufferElement;
	//		communicationFirmwareInstance_->waitClear(bufferElement, STRIP_SC_CSR,
	// 0x80000000); 		uint32_t registerHeader = 0;
	//		//FIXME This must go in the FSSRROCDefinitions stuff
	//		//if (it->first != "RejectHits" && it->first != "SendData")
	//		if (it->first == "VTp0")
	//		{
	//			//communicationFirmwareInstance_->write(bufferElement,
	// ChannelFIFOAddress[channel], it->second.second); 			registerHeader =
	// FSSRROCDefinitions::makeDACReadHeader(rocStream.getFEWROCAddress(), it->first);
	//			//Insert channel
	//			BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
	////		}
	////		else
	////		if (it->first == "SendData")
	////		{
	////			if (it->second.second == 1 || it->second.second == 2)
	////			{
	////				registerHeader =
	/// FSSRROCDefinitions::makeDACSetHeader(rocStream.getFEWROCAddress(), it->first); /
	/////Insert channel /				BitManipulator::insertBits(registerHeader, 1, 16
	///+ channel, 1); /			} /			else if (it->second.second == 0 ||
	/// it->second.second == 5) /			{ /				registerHeader =
	/// FSSRROCDefinitions::makeDACResetHeader(rocStream.getFEWROCAddress(), it->first); /
	/////Insert channel /				BitManipulator::insertBits(registerHeader, 1, 16
	///+ channel, 1); /			} /			else
	////				__COUT__ << "Register value for : " <<
	/// it->first /				<< " doesn't have a value I expect -> value = " /
	///<< it->second.second << std::endl;
	////
	////		}
	//
	//	    __COUT__ << "Register: " << it->first << " value: " <<
	// it->second.second << std::hex << " -> Data: " << registerHeader << std::dec <<
	// std::endl; 		communicationFirmwareInstance_->write(bufferElement, STRIP_SC_CSR,
	// registerHeader); 		communicationFirmwareInstance_->waitClear(bufferElement,
	// STRIP_SC_CSR, 0x80000000);
	//
	//		//buffer.push_back(bufferElement);
	//		buffer += bufferElement;
	//		}
	//		//break;
	//	}
	for(DACList::const_iterator it = rocDACs.getDACList().begin();
	    it != rocDACs.getDACList().end();
	    it++)
	{
		std::string bufferElement;
		communicationFirmwareInstance_->waitClear(
		    bufferElement, STRIP_SC_CSR, 0x80000000, false);
		uint32_t registerHeader = 0;
		// FIXME This must go in the FSSRROCDefinitions stuff
		if(it->first != "RejectHits" && it->first != "SendData")
		{
			communicationFirmwareInstance_->write(
			    bufferElement, ChannelFIFOAddress[channel], it->second.second, false);
			registerHeader = FSSRROCDefinitions::makeDACWriteHeader(
			    rocStream.getFEWROCAddress(), it->first);
			// Insert channel
			BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
		}
		else
		{
			if(it->second.second == 1 || it->second.second == 2)
			{
				registerHeader = FSSRROCDefinitions::makeDACSetHeader(
				    rocStream.getFEWROCAddress(), it->first);
				// Insert channel
				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
			}
			else if(it->second.second == 0 || it->second.second == 5)
			{
				registerHeader = FSSRROCDefinitions::makeDACResetHeader(
				    rocStream.getFEWROCAddress(), it->first);
				// Insert channel
				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
			}
			else
				__COUT__ << "Register value for : " << it->first
				         << " doesn't have a value I expect -> value = "
				         << it->second.second << std::endl;
		}

		// __COUT__ << "Register: " << it->first << " value: " <<
		// it->second.second << std::hex << " -> Data: " << registerHeader << std::dec <<
		// std::endl;
		communicationFirmwareInstance_->write(
		    bufferElement, STRIP_SC_CSR, registerHeader, false);
		communicationFirmwareInstance_->waitClear(
		    bufferElement, STRIP_SC_CSR, 0x80000000, false);

		// buffer.push_back(bufferElement);
		buffer += bufferElement;
		// break;
	}
	__COUT__ << "BufferOUTsize: " << buffer.size() << std::endl;
}

//==============================================================================
void FSSRFirmwareBase::makeDACBuffer(std::vector<std::string>& buffer,
                                     unsigned int              channel,
                                     const ROCStream&          rocStream)
{
	// __COUT__ << "\tMaking DAC Buffer" << std::endl;

	int          limitCount        = 0;
	unsigned int singleVectorCount = 0;

	std::string alternateBuffer;

	// __COUT__ << "Channel: " << channel << std::endl;
	const ROCDACs& rocDACs = rocStream.getROCDACs();
	// __COUT__ << "Number of DACs: " << rocDACs.getDACList().size()
	// << std::endl;
	std::string bufferElement;
	// FIXME My
	//	for (const auto it: rocDACs.getDACList())
	for(const std::pair<std::string, std::pair<unsigned int, unsigned int> >& it :
	    rocDACs.getDACList())
	//	for (std::map<std::string, std::pair<unsigned int, unsigned int> >::const_iterator
	// it = rocDACs.getDACList().begin(); it != rocDACs.getDACList().end(); it++)
	{
		// __COUT__ << "Register?" << std::endl;
		//		if (it.first != "ActiveLines") continue;
		//		__COUT__ << "Register-1: " << it->first << " val: " <<
		// it->second.first  << " = " << it->second.second << std::endl;
		communicationFirmwareInstance_->waitClear(
		    bufferElement, STRIP_SC_CSR, 0x80000000, false);
		uint32_t registerHeader = 0;
		// FIXME This must go in the FSSRROCDefinitions stuff
		// __COUT__ << "Register0: " << it.first << std::endl;
		if(it.first != "RejectHits" && it.first != "SendData")
		{
			// __COUT__ << "Register1: " << it.first << "Channel: " <<
			// channel << " fifo address: " << ChannelFIFOAddress[channel] << std::endl;
			communicationFirmwareInstance_->write(
			    bufferElement, ChannelFIFOAddress[channel], it.second.second, false);
			registerHeader = FSSRROCDefinitions::makeDACWriteHeader(
			    rocStream.getFEWROCAddress(), it.first);
			// Insert channel
			BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
		}
		else
		{
			// __COUT__ << "Register2: " << it.first << std::endl;
			if(it.second.second == 1 || it.second.second == 2)
			{
				registerHeader = FSSRROCDefinitions::makeDACSetHeader(
				    rocStream.getFEWROCAddress(), it.first);
				// Insert channel
				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
			}
			else if(it.second.second == 0 || it.second.second == 5)
			{
				registerHeader = FSSRROCDefinitions::makeDACResetHeader(
				    rocStream.getFEWROCAddress(), it.first);
				// Insert channel
				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
			}
			else
				__COUT__ << "Register value for : " << it.first
				         << " doesn't have a value I expect -> value = "
				         << it.second.second << std::endl;
		}
		// __COUT__ << "Register: " << it.first << " value: " <<
		// it.second.second << std::hex << " -> Data: " << registerHeader << std::dec <<
		// std::endl;
		communicationFirmwareInstance_->write(
		    bufferElement, STRIP_SC_CSR, registerHeader, false);
		communicationFirmwareInstance_->waitClear(
		    bufferElement, STRIP_SC_CSR, 0x80000000, false);

		// __COUT__ << "Register3: " << it.first << std::endl;
		// alternateBuffer += bufferElement;
		limitCount++;
		singleVectorCount++;

		if(limitCount == STIB_DAC_WRITE_MAX)
		{
			__COUT__ << "\tBuffer length:" << bufferElement.size() << std::endl;
			buffer.push_back(bufferElement);
			limitCount = 0;
			bufferElement.clear();
		}
		else if(singleVectorCount ==
		        rocDACs.getDACList().size())  // case for incomplete packet
		{
			buffer.push_back(bufferElement);
		}

		// buffer.push_back(bufferElement);
		// break;
	}
	// __COUT__ << "\tDone making DAC Buffer" << std::endl;
}

//==============================================================================
void FSSRFirmwareBase::makeMaskBuffer(std::string&     buffer,
                                      unsigned int     channel,
                                      const ROCStream& rocStream)
{
	// __COUT__ << "Making mask! " << std::endl;
	makeMaskBuffer(buffer, channel, rocStream, "Kill");
	//    makeMaskSequence(buffer, channel, rocStream, "Inject");
}

//==============================================================================
void FSSRFirmwareBase::makeMaskBuffer(std::string&       buffer,
                                      unsigned int       channel,
                                      const ROCStream&   rocStream,
                                      const std::string& registerName)
{
	int                chipId = rocStream.getFEWROCAddress();
	const std::string& mask   = rocStream.getROCMask();
	// __COUT__ << "\tMaking mask! Length = " << mask.length() <<
	// std::endl; 	__COUT__ << "\tMask length: " << mask.length() <<
	// std::endl; 	__COUT__ << "\tMask: " << mask << std::endl;

	unsigned int data[4] = {0, 0, 0, 0};

	char val;
	// int j = 0;
	for(unsigned int d = 0; d < 4; d++)
	{  // d goes from 0 to 4. 4 bytes
		// __COUT__ << "---------------------" << d <<
		// "-------------------" << std::endl;
		for(unsigned int m = 0; m < 8 * 4; m++)
		{  // m goes from 0 to 31, since each byte has 8 bits, there are 32 bits
			val = mask[(8 * 4 * d) + m];  // assigns to val the value of the corresponding
			                              // bit. 0-31, 32-63, 64-95, 96-127. it goes
			                              // through each of the 128 bits
			// __COUT__ << "---------------------" << j++ <<
			// std::endl;  __COUT__ << "data[" << d << "] before: " <<
			// std::hex << data[d] << std::dec << std::endl;
			data[d] |= (unsigned int)atoi(&val) << (8 * 4 - 1 - m);
			// __COUT__ << "(unsigned int) atoi(&val): " << (unsigned
			// int) atoi(&val) << std::endl;  __COUT__ << "data[" << d
			// << "] after: " << std::hex << data[d] << std::dec << std::endl;  std::cout
			// <<
			// __COUT_HDR_FL__ << std::hex << "D: " << data[d] << " Val: " << (unsigned
			// int)atoi(&val) << " index: " << std::dec << (8*4-1-m) << " bit: " <<
			// mask[(8*4*d)+m] << std::dec << std::endl;
		}
		//
	}

	int           i;
	unsigned int  w;
	unsigned char len     = 4;
	unsigned char addr    = 17;  // Kill
	unsigned char bitMask = 1 << channel;
	unsigned char inst    = WRITE;

	communicationFirmwareInstance_->waitClear(buffer, STRIP_SC_CSR, 0x80000000, false);

	for(i = 0; i < 4; i++)
		// write(buffer, STRIP_SCI + 4 * i, data[i]);
		communicationFirmwareInstance_->write(
		    buffer, STRIP_SCI + 4 * (4 - i - 1), data[i], false);

	w = 0x80000000 | (len << 24) | (bitMask << 16) | (inst << 10) | (addr << 5) | chipId;

	communicationFirmwareInstance_->write(buffer, STRIP_SC_CSR, w, false);

	communicationFirmwareInstance_->waitClear(buffer, STRIP_SC_CSR, 0x80000000, false);
}

//==============================================================================
void FSSRFirmwareBase::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
                                        unsigned int                channel,
                                        const ROCStream&            rocStream)
{
	__COUT__ << "Making mask! " << std::endl;
	makeMaskSequence(sequence, channel, rocStream, "Kill");
	//    makeMaskSequence(sequence, channel, rocStream, "Inject");
}

//==============================================================================
void FSSRFirmwareBase::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
                                        unsigned int                channel,
                                        const ROCStream&            rocStream)
{
	__COUT__ << "Making mask! " << std::endl;
	makeMaskSequence(sequence, channel, rocStream, "Kill");
	//    makeMaskSequence(channel,rocStream,sequence,"Inject");
}

//==============================================================================
void FSSRFirmwareBase::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
                                        unsigned int                channel,
                                        const ROCStream&            rocStream,
                                        const std::string&          registerName)
{
	int         chipId = rocStream.getFEWROCAddress();  // 9, 14 or 21 bcast
	std::string mask   = rocStream.getROCMask();
	__COUT__ << "Mask length: " << mask.length() << std::endl;

	uint64_t    uInt64Data = 0;
	std::string stringData = "";

	// have to manually set every bit for mask writes.
	// reset fifo
	// download every bit (shift_in and shift_ctrl) to fifo (setup muxes and control
	// lines)  configure muxes for readout  commence readout

	// FIFO Controls - byte 5 (7:0)
	// 7 - FIFO Clock
	// 6 - Shift Ctrl bit
	// MASK_CELL_H - Shift In bit
	// MUX Controls - byte 0 (7:0)
	// 7 - Read En/Output MUX sel (1 for masks)
	// 1 - Write En for mux
	// 0 - Reset Fifo

	// reset fifo
	BitManipulator::insertBits(uInt64Data, 0x1, 56, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);

	// configure fifo for write
	BitManipulator::insertBits(uInt64Data, 0x2, 56, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);

	// Download every bit (shift_in and shift_control) to fifo (setup muxes and control
	// lines)  Bit 7 of data is FIFO clock, bit 6 is shift_control, bit 5 is shift_in
	// start bits (ctrl 0, data 0 => ctrl 1, data 0)

	BitManipulator::insertBits(uInt64Data, 0x40, 16, 8);  //(0<<7) | (1<<6) | (0<<5)
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
	BitManipulator::insertBits(
	    uInt64Data,
	    0xc0,
	    16,
	    8);  //(1<<7) | (1<<6) | (0<<5) -> clock the data in the fifo
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);

	stringData = FSSRROCDefinitions::makeMaskWriteCommand(chipId, registerName, mask);

	uint8_t data;
	for(unsigned int s = 0; s < stringData.length(); s++)
		for(int b = 8 - 1; b >= 0 && (s * 8 + 8 - b < 13 + 128); b--)
		{
			// __COUT__ << "S: " << s << " val: " <<
			// stringData.data()[s] << " b: " << b << " bit: " <<
			// ((stringData.data()[s]>>b)&1) << std::endl;
			data = 0x40 | (((stringData.data()[s] >> b) & 1) << 5);
			BitManipulator::insertBits(uInt64Data, (uint64_t)data, 16, 8);
			sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
			data |= 0x80;
			BitManipulator::insertBits(uInt64Data, (uint64_t)data, 16, 8);
			sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
		}

	// reset Shift Control
	BitManipulator::insertBits(uInt64Data, 0x0, 16, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
	BitManipulator::insertBits(uInt64Data, 0x80, 16, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);

	// configure muxes for readout
	BitManipulator::insertBits(uInt64Data, 0x0, 56, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);

	// commence readout
	BitManipulator::insertBits(uInt64Data, 0x80, 56, 8);
	sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
}

//==============================================================================
void FSSRFirmwareBase::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
                                        unsigned int                channel,
                                        const ROCStream&            rocStream,
                                        const std::string&          registerName)
{
}

//==============================================================================
std::string FSSRFirmwareBase::readCSRRegister()
{
	std::string buffer;
	// __COUT__ << "FSSR readCSRRegister" << std::endl;
	communicationFirmwareInstance_->read(buffer, STRIP_CSR);
	return buffer;
}

//==============================================================================
std::string FSSRFirmwareBase::readSCCSRRegister()
{
	std::string buffer;
	std::cout << __COUT_HDR_FL__ << "FSSR readSCCSRRegister" << std::endl;
	communicationFirmwareInstance_->read(buffer,STRIP_SC_CSR);
	return buffer;
}

//========================================================================================================================
std::string FSSRFirmwareBase::readTrimCSRRegister(void)
{
	std::string buffer;
	std::cout << __COUT_HDR_FL__ << "FSSR readTrimCSRRegister" << std::endl;
	communicationFirmwareInstance_->read(buffer,STRIP_TRIM_CSR);
	return buffer;
}

//========================================================================================================================
void FSSRFirmwareBase::setFrequencyFromClockState(std::string& buffer, double frequency)
{
	// __COUT__ << "Setting up clock frequency!!!" << std::endl;

	int    quotient;
	int    numerator;
	int    denominator;
	double realClockFrequency;

	if(BitManipulator::readBits(stripCSRRegisterValue_, 17, 1) ==
	   1)  // if fastBCO is enabled
		quotient = 4;
	else  // normal cases
		quotient = 8;

	if(isClockStateExternal())  // base freq: 54MHz
	{
		realClockFrequency =
		    EXTERNAL_CLOCK_FREQUENCY / quotient;  // this is the REAL frequency being used
	}
	else  // base freq: 66.667MHz
	{
		realClockFrequency =
		    INTERNAL_CLOCK_FREQUENCY / quotient;  // this is the REAL frequency being used
	}

	double factor = frequency / realClockFrequency;

	// The device needs the denominator and the denominator to be load into a 5 bit
	// register  It will take two initial numerator and denominator bigger than necessary
	// (to do not loose precision)  and divide them for their gcd. If they still do not
	// fit in 5 bit, they are trunked (divided by 2)  untill they are less than 32
	numerator =
	    factor * 100;  // we will work with 2 digits precision after the decimal point
	denominator = 100;

	do
	{
		// We will need the GCD at some point in order to simplify fractions //taken from
		// other sources
		int gcd  = numerator;
		int rest = denominator;
		int tmp;

		while(rest != 0)
		{
			tmp  = rest;
			rest = gcd % tmp;
			gcd  = tmp;
		}
		// Done finding the GCD

		if(gcd == 1)  // if there's no GCD, just divide by 2 to find the nearest
		              // approximation with less bits
		{
			numerator /= 2;
			denominator /= 2;
		}
		else
		{
			numerator /= gcd;
			denominator /= gcd;
		}

	} while(denominator >= 32 || numerator >= 32);

	// numerator = 2;
	// denominator = 1;
	// __COUT__ << "Numerator: " << numerator << std::endl;
	// __COUT__ << "Denominator: " << denominator << std::endl;
	setFrequencyRatio(buffer, numerator, denominator);
	// __COUT__ << "Done with clock frequency setup!!!" << std::endl;
}
//==============================================================================
bool FSSRFirmwareBase::isClockStateExternal()  // returns true if the clock state is
                                               // External
{
	if(BitManipulator::readBits(stripCSRRegisterValue_, 16, 1) == 1)
		return true;
	else
		return false;
}

//==============================================================================
void FSSRFirmwareBase::setCSRRegister(uint32_t total) { stripCSRRegisterValue_ = total; }

//==============================================================================
void FSSRFirmwareBase::setPacketSizeStripCSR(uint32_t size)
{
	if(size > 31)
		__COUT__ << "ERROR: Maximum packet size is 31 while here you are trying to set "
		         << size << " packets!" << std::endl;
	BitManipulator::insertBits(stripCSRRegisterValue_, size, 3, 5);
	// write(buffer,STRIP_CSR, stripSCRRegisterValue_);
}

//==============================================================================
void FSSRFirmwareBase::enableChannelsStripCSR(bool channel0,
                                              bool channel1,
                                              bool channel2,
                                              bool channel3,
                                              bool channel4,
                                              bool channel5)
{
	BitManipulator::insertBits(stripCSRRegisterValue_,
	                           ((uint32_t)channel0) + ((uint32_t)channel1 << 1) +
	                               ((uint32_t)channel2 << 2) + ((uint32_t)channel3 << 3) +
	                               ((uint32_t)channel4 << 4) + ((uint32_t)channel5 << 5),
	                           8,
	                           6);
}

//==============================================================================
void FSSRFirmwareBase::setExternalBCOClockSourceStripCSR(std::string clockSource)
{
	if(clockSource == "External")
		BitManipulator::insertBits(stripCSRRegisterValue_, 1, 16, 1);
	else if(clockSource == "Internal")
		BitManipulator::insertBits(stripCSRRegisterValue_, 0, 16, 1);
}

//==============================================================================
// void FSSRFirmwareBase::setHaltStripCSR(bool set)
//{
//	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) set, 17, 1);
//}

//==============================================================================
void FSSRFirmwareBase::armBCOResetCSR(void)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 19, 1);
}

//==============================================================================
void FSSRFirmwareBase::flushBuffersStripCSR(void)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 20, 1);
}

//==============================================================================
void FSSRFirmwareBase::resetTriggerCounterStripCSR(std::string& buffer)
{
	// Ryan's firmware is too fast so I need to make sure he understand the 1!!!!
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);

	BitManipulator::insertBits(stripCSRRegisterValue_, 0, 21, 1);
	communicationFirmwareInstance_->write(
	    buffer, STRIP_CSR, stripCSRRegisterValue_, false /*clearBuffer*/);
}

//==============================================================================
void FSSRFirmwareBase::resetBCOCounterStripCSR(void)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, 1, 22, 1);
}

//==============================================================================
void FSSRFirmwareBase::enableTriggerStripCSR(bool enable)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)enable, 23, 1);
}

//==============================================================================
void FSSRFirmwareBase::sendTriggerDataStripCSR(bool send)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)send, 24, 1);
}

//==============================================================================
void FSSRFirmwareBase::sendTriggerNumberStripCSR(bool send)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)send, 25, 1);
}

//==============================================================================
void FSSRFirmwareBase::sendBCOStripCSR(bool send)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)send, 26, 1);
}

//==============================================================================
void FSSRFirmwareBase::enableStreamStripCSR(bool enable)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)enable, 27, 1);
}

//==============================================================================
void FSSRFirmwareBase::resetDCMStripCSR(bool clear)
{
	BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t)clear, 31, 1);
}

//==============================================================================
uint32_t FSSRFirmwareBase::waitDCMResetStripCSR(void)
{
	uint32_t bitToCheck = 0;
	BitManipulator::insertBits(bitToCheck, 1, 31, 2);
	return bitToCheck;
}

//==============================================================================
void FSSRFirmwareBase::resetDAC(void)
{
	BitManipulator::insertBits(stripResetRegisterValue_, 1, 27, 1);
}

//==============================================================================
void FSSRFirmwareBase::resetLink(bool channel0,
                                 bool channel1,
                                 bool channel2,
                                 bool channel3,
                                 bool channel4,
                                 bool channel5)
{
	stripResetRegisterValue_ = 0;
	BitManipulator::insertBits(stripResetRegisterValue_,
	                           ((uint32_t)channel0) + ((uint32_t)channel1 << 1) +
	                               ((uint32_t)channel2 << 2) + ((uint32_t)channel3 << 3) +
	                               ((uint32_t)channel4 << 4) + ((uint32_t)channel5 << 5),
	                           0,
	                           6);
	BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1);
	/*
	 write(buffer, STRIP_RESET, (1<<28) +
	 ((uint32_t)channel0) +
	 ((uint32_t)channel1<<1) +
	 ((uint32_t)channel2<<2) +
	 ((uint32_t)channel3<<3) +
	 ((uint32_t)channel4<<4) +
	 ((uint32_t)channel5<<5)
	 );
	 */
}

//==============================================================================
void FSSRFirmwareBase::clearErrors(bool channel0,
                                   bool channel1,
                                   bool channel2,
                                   bool channel3,
                                   bool channel4,
                                   bool channel5)
{
	stripResetRegisterValue_ = 0;
	BitManipulator::insertBits(stripResetRegisterValue_,
	                           ((uint32_t)channel0) + ((uint32_t)channel1 << 1) +
	                               ((uint32_t)channel2 << 2) + ((uint32_t)channel3 << 3) +
	                               ((uint32_t)channel4 << 4) + ((uint32_t)channel5 << 5),
	                           0,
	                           6);
	BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1);
}

//==============================================================================
void FSSRFirmwareBase::clearFIFO(bool channel0,
                                 bool channel1,
                                 bool channel2,
                                 bool channel3,
                                 bool channel4,
                                 bool channel5)
{
	stripResetRegisterValue_ = 0;
	BitManipulator::insertBits(stripResetRegisterValue_,
	                           ((uint32_t)channel0) + ((uint32_t)channel1 << 1) +
	                               ((uint32_t)channel2 << 2) + ((uint32_t)channel3 << 3) +
	                               ((uint32_t)channel4 << 4) + ((uint32_t)channel5 << 5),
	                           0,
	                           6);
	BitManipulator::insertBits(stripResetRegisterValue_, 1, 30, 1);
}

//==============================================================================
void FSSRFirmwareBase::resetChip(bool channel0,
                                 bool channel1,
                                 bool channel2,
                                 bool channel3,
                                 bool channel4,
                                 bool channel5)
{
	stripResetRegisterValue_ = 0;
	BitManipulator::insertBits(stripResetRegisterValue_,
	                           ((uint32_t)channel0) + ((uint32_t)channel1 << 1) +
	                               ((uint32_t)channel2 << 2) + ((uint32_t)channel3 << 3) +
	                               ((uint32_t)channel4 << 4) + ((uint32_t)channel5 << 5),
	                           0,
	                           6);
	BitManipulator::insertBits(stripResetRegisterValue_, 1, 31, 1);
}

//==============================================================================
void FSSRFirmwareBase::setFrequencyRatio(std::string& buffer,
                                         int          numerator,
                                         int          denominator)
{
	// The device need to load numerator minus one and denominator minus one, with an
	// internal address of 0x50 and 052 respectively
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_BCO_DCM,
	    0x80500000 + (numerator - 1));  //  Set BCOCLK numerator // was 0x80500003
	communicationFirmwareInstance_->waitClear(
	    buffer,
	    STRIP_BCO_DCM,
	    0xf0000000);  //  Wait DCM write to finish // was 0x80000000

	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_BCO_DCM,
	    0x80520000 + (denominator - 1));  //  Set BCOCLK denominator // was 0x80520001
	communicationFirmwareInstance_->waitClear(
	    buffer, STRIP_BCO_DCM, 0xf0000000);  //  Wait DCM write to finish - BCO frequency
	                                         //  should be 13.513 MHz // was 0x80000000
}

//==============================================================================
void FSSRFirmwareBase::BCOOffset(uint32_t offset)
{
	BitManipulator::insertBits(stripTriggerCSRRegisterValue_, offset, 0, 4);
}

//==============================================================================
void FSSRFirmwareBase::selectSpyFIFO(uint32_t input)
{
	BitManipulator::insertBits(stripTriggerCSRRegisterValue_, input, 4, 3);
}

//==============================================================================
void FSSRFirmwareBase::halt(bool halt)
{
	BitManipulator::insertBits(stripTriggerCSRRegisterValue_, (uint32_t)halt, 7, 1);
}

//==============================================================================
void FSSRFirmwareBase::configureStripTriggerUnbiased(std::string& buffer)
{
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_TRIG_UNBIASED,
	    0x002805c,
	    false /*clearBuffer*/);  //  Configure unbiased trigger
}

//==============================================================================
void FSSRFirmwareBase::configureTriggerInputs(std::string& buffer)
{
	//	communicationFirmwareInstance_->write(buffer, STRIP_TRIG_INPUT_0, 0x3f440000); //
	// FSSR GOTHIT trigger input channel 0,1
	//	communicationFirmwareInstance_->write(buffer, STRIP_TRIG_INPUT_1, 0x3f440000); //
	// FSSR GOTHIT trigger input channel 2,3
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_TRIG_INPUT_0,
	    0x0,
	    false /*clearBuffer*/);  //  FSSR GOTHIT trigger input channel 0,1
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_TRIG_INPUT_1,
	    0x0,
	    false /*clearBuffer*/);  //  FSSR GOTHIT trigger input channel 2,3
	communicationFirmwareInstance_->write(
	    buffer,
	    STRIP_TRIG_INPUT_2,
	    0x0,
	    false /*clearBuffer*/);  //  FSSR GOTHIT trigger input channel 4,5
}

//==============================================================================
std::string FSSRFirmwareBase::resetSlaveBCO(void)
{
	std::string buffer;
	/*TODO:make unambiguous by casting to uint32_t*/
	communicationFirmwareInstance_->write(
	    buffer, (uint32_t)0xc5000000, (uint32_t)0x00000008);
	return buffer;
}

/*
 //==============================================================================
 void FSSRFirmwareBase::chipID(uint32_t size)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, size, 0, 5);
 }

 //==============================================================================
 void FSSRFirmwareBase::addressSlowControls(uint32_t size)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, size, 5, 5);
 }

 //==============================================================================
 void FSSRFirmwareBase::instructionSlowControls(uint32_t size)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, size, 10, 3);
 }

 //==============================================================================
 void FSSRFirmwareBase::channelreadSelect(uint32_t size)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, size, 13, 3);
 }

 //==============================================================================
 void FSSRFirmwareBase::channelMask(uint32_t size)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, size, 16, 8);
 }

 //==============================================================================
 void FSSRFirmwareBase::bitsLength(uint32_t length)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, length, 26, 3);
 }


 //==============================================================================
 void FSSRFirmwareBase::syncFallingBCO(bool sync)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)sync, 28, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::syncRisingBCO(bool sync)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)sync, 29, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::setRaw(bool set)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)set, 30, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::initSlowControls(bool init)
 {
 BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)init, 31, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::resetCount(bool reset)
 {
 BitManipulator::insertBits(stripAnalysisCSRRegisterValue_, (uint32_t)reset, 30, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::setBCO_0(uint32_t void FSSRFirmwareBase::BCOOffset(uint32_t
 offset)
 {
 BitManipulator::insertBits(stripTrigCSRRegisterValue_, offset, 0, 4);
 }input)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, input, 0, 8);
 }

 //==============================================================================
 void FSSRFirmwareBase::setBCO_1(uint32_t input)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, input, 8, 8);
 }

 //==============================================================================
 void FSSRFirmwareBase::trimFracBCO_0(uint32_t input)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, input, 16, 4);
 }

 //==============================================================================
 void FSSRFirmwareBase::trimFracBCO_1(uint32_t input)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, input, 20, 4);
 }

 //==============================================================================
 void FSSRFirmwareBase::enable_0(bool enable)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, (uint32_t)enable, 28, 1);
 }

 //==============================================================================
 void FSSRFirmwareBase::enable_1(bool enable)
 {
 BitManipulator::insertBits(trigInputRegisterValue_, (uint32_t)enable, 29, 1);
 }

 */
