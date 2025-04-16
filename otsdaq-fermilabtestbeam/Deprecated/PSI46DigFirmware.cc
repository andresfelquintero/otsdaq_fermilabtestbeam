//#include "otsdaq-fermilabtestbeam/DAQHardware/PSI46DigFirmware.h"
//#include "otsdaq-fermilabtestbeam/DAQHardware/PSI46DigFirmwareDefinitions.h"
//#include "otsdaq/BitManipulator/BitManipulator.h"
//#include "otsdaq-fermilabtestbeam/DetectorHardware/PSI46DigROCDefinitions.h"
//#include "otsdaq/MessageFacility/MessageFacility.h"
//#include "otsdaq/Macros/CoutMacros.h"
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <iostream>
//#include <cstdlib>
//
// using namespace ots;
//
//
////==============================================================================
// PSI46DigFirmware::PSI46DigFirmware(unsigned int version, std::string type) :
//        FrontEndFirmwareBase(version),
//        stripCSRRegisterValue_(0)
//{
//	protocolInstance_ = FrontEndFirmwareBase::getInstance(type,version);
//	assert(protocolInstance_ != NULL);
//	//returns either new OtsUDPFirmware or new PurdueFirmwareCore,
//	//now we can call write/read etc with protocol->write, protocol->read, etc
//}
//
////==============================================================================
// PSI46DigFirmware::~PSI46DigFirmware(void)
//{
//	delete protocolInstance_;
//	protocolInstance_= NULL;
//}
//
////==============================================================================
// int PSI46DigFirmware::init(void)
//{
//    return 0;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::configureClocks(std::string source, double frequency)
//{
//    std::cout << __COUT_HDR_FL__ << "Writing Clock configuration!" << std::endl;
//
//    std::string buffer;
//    //NoNeedNowwrite(buffer, ETHIO_DESTINATION_PORT, 0x0000b798); //  Listen port for
//    ethio stuff
//
//    setPacketSizeStripCSR(6);
//    setExternalBCOClockSourceStripCSR(source); //(source)
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Reset CSR -
//    reset trigger counter, external 27 MHz clock
//
//    resetDCMStripCSR(true);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Set reset
//    to DCM
//
//    resetDCMStripCSR(false);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Clear reset
//    to DCM
//
//    ;
//    protocolInstance_->waitClear(buffer, STRIP_CSR, waitDCMResetStripCSR()); //  Wait
//    for DCM to lock
//
//    protocolInstance_->write(buffer, STRIP_TRIM_CSR, 0x00002000); //  MCLKB edge for
//    channel 5 // was 0x00002000
//
//    setFrequencyFromClockState(buffer, frequency);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    resetDCMStripCSR(true);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    resetDCMStripCSR(false);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    protocolInstance_->waitClear(buffer, STRIP_CSR, waitDCMResetStripCSR()); //  Wait
//    for DCM to lock std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue :" <<
//    std::hex << stripCSRRegisterValue_ << std::dec << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::resetDetector(int channel)
//{
//    std::cout << __COUT_HDR_FL__ << "Resetting detector!" << std::endl;
//    std::string buffer;
//    if (channel == -1)//reset all channels
//    {
//        //protocolInstance_->write(buffer,STRIP_RESET,0xd000003f);                  //
//        Issue reset protocolInstance_->write(buffer, STRIP_RESET, 0xf000003f); //  Issue
//        reset // was 0xf000003f protocolInstance_->waitClear(buffer, STRIP_RESET,
//        0xf0000000); //  Wait for reset to complete // was 0xf0000000
//    } else
//    {
//        protocolInstance_->write(buffer, STRIP_RESET, 0xf000003f); //  Issue reset
//        protocolInstance_->waitClear(buffer, STRIP_RESET, 0xf0000000); //  Wait for
//        reset to complete
//    }
//
//    return buffer;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::enableTrigger(void)
//{
//    std::string buffer;
//    std::cout << __COUT_HDR_FL__ << "Enabling Trigger!!!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in :" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl;
//
//    setHaltStripCSR(1);//WARNING THIS IS CLEARED BY THE MASTER BUT IF THERE IS NO MASTER
//    NOTHING WORKS UNLESS THE BIT IS UNSET
//    //setHaltStripCSR(0);//WARNING THIS IS CLEARED BY THE MASTER BUT IF THERE IS NO
//    MASTER NOTHING WORKS UNLESS THE BIT IS UNSET sendTriggerDataStripCSR(true);
//    sendTriggerNumberStripCSR(true);
//    sendBCOStripCSR(true);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    stripTriggerCSRRegisterValue_ = 0;
//    BCOOffset(4);
//    protocolInstance_->write(buffer, STRIP_TRIG_CSR, stripTriggerCSRRegisterValue_); //
//    BCO offset // was 0x00000004
//
//    //  write(buffer,STRIP_TRIG_INPUT_0,0x1f060040);  //  PSI46Dig GOTHIT trigger input
//    - timed in for the 27 MHz external clock
//    //  write(buffer,STRIP_TRIG_INPUT_3,0x3f874040);  //  Unbiased trigger input +
//    external trigger
//
//    configureStripTriggerUnbiased(buffer);
//
//    configureTriggerInputs(buffer);
//
//    //FIXME for IP .36 the number to set is 0x20401000
//
//    if (version_ == 1)
//        protocolInstance_->write(buffer, STRIP_TRIG_INPUT_3, 0x20401000); // Turn on
//        streaming hits along with BCO data
//    else if (version_ == 2)
//        protocolInstance_->write(buffer, STRIP_TRIG_INPUT_3, 0x20301000); // Turn on
//        streaming hits along with BCO data
//    else
//    {
//    	std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "what version is this?" <<
//    version_ << std::endl;
//	assert(0);
//    }
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue out:" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done enabling Trigger!!!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::resetBCO(void)
//{
//    std::cout << __COUT_HDR_FL__ << "Reset BCO!!!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in :" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl; std::string buffer;
//
//    resetTriggerCounterStripCSR(buffer);
//    //write(buffer, STRIP_CSR, stripCSRRegisterValue_);//the write is done in the reset
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue :" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl;
//
//    //msg->Write(STRIP_SC_CSR,0x90000b95|(chmask<<16));
//    //write(buffer,STRIP_SC_CSR,0x900f0b95);//  This is the <SCR,set> command with the
//    bit set to sync SHIFT with BCO=0.
//
//
//    enableBCOStripCSR(true);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue out:" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done reset BCO!!!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::startStream(bool channel0, bool channel1, bool channel2,
// bool channel3, bool channel4, bool channel5)
//{
//    std::cout << __COUT_HDR_FL__ << "Start Stream!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in:" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl; std::string buffer;
//
//    std::cout << __COUT_HDR_FL__ << " channel0 " << channel0 << " channel1 " << channel1
//    << " channel2 " << channel2 << " channel3 " << channel3 << " channel4 " << channel4
//    << " channel5 " << channel5 << std::endl;
//
//    enableChannelsStripCSR(channel0, channel1, channel2, channel3, channel4, channel5);
//    //    if (version_ == 1)
//    //        enableChannelsStripCSR(true, true, true, true, false, false);
//    //    else if (version_ == 2)
//    //        enableChannelsStripCSR(true, true, true, true, true, true);
//
//    enableStreamStripCSR(true); //  Turn on streaming hits along with BCO data // was
//    0x0f000f30 protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue out:" << std::hex <<
//    stripCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done start Stream!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string PSI46DigFirmware::stopStream(void)
//{
//    std::string buffer;
//    enableChannelsStripCSR(false, false, false, false, false, false);
//    enableStreamStripCSR(false);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//    return buffer;
//}
//
////==============================================================================
// void PSI46DigFirmware::makeDACSequence(FirmwareSequence<uint64_t>& sequence,
//                                     unsigned int channel, const ROCStream& rocStream)
//{
//    const ROCDACs& rocDACs = rocStream.getROCDACs();
//    for (DACList::const_iterator it = rocDACs.getDACList().begin(); it
//            != rocDACs.getDACList().end(); it++)
//    {
//        //if(it->first != "SendData" && it->first != "RejectHits") continue;
//        uint64_t data = PSI46DigROCDefinitions::makeDACWriteCommand(
//                            rocStream.getFEWROCAddress(), it->first, it->second.second);
//        sequence.pushBack(ChannelFIFOAddress[channel], data);
//        sequence.pushBack(ChannelFIFOAddress[channel],
//                          BitManipulator::insertBits(data, (uint64_t) 0x48, 56, 8));
//        //set WRITE
//        sequence.pushBack(ChannelFIFOAddress[channel],
//                          BitManipulator::insertBits(data, (uint64_t) 1, 60, 1));
//        //clr WRITE
//        sequence.pushBack(ChannelFIFOAddress[channel],
//                          BitManipulator::insertBits(data, (uint64_t) 0, 60, 1));
//        //clr TALK
//        sequence.pushBack(ChannelFIFOAddress[channel],
//                          BitManipulator::insertBits(data, (uint64_t) 0, 62, 1));
//        sequence.pushBack(ChannelFIFOAddress[channel],
//                          BitManipulator::insertBits(data, (uint64_t) 0x40, 56, 8));
//        //break;
//    }
//}
//
////==============================================================================
// void PSI46DigFirmware::makeDACSequence(FirmwareSequence<uint32_t>& sequence,
//                                     unsigned int channel, const ROCStream& rocStream)
//{
//    const ROCDACs& rocDACs = rocStream.getROCDACs();
//    for (DACList::const_iterator it = rocDACs.getDACList().begin(); it
//            != rocDACs.getDACList().end(); it++)
//    {
//        /*RYAN
//         //if(it->first != "SendData" && it->first != "RejectHits") continue;
//         uint64_t data =
//         PSI46DigROCDefinitions::makeDACWriteCommand(rocStream.getFEWROCAddress(),
//         it->first, it->second.second); sequence.pushBack(ChannelFIFOAddress[channel],
//         data); sequence.pushBack(ChannelFIFOAddress[channel],
//         BitManipulator::insertBits(data,(uint32_t)0x48,56,8));
//         //set WRITE
//         sequence.pushBack(ChannelFIFOAddress[channel],
//         BitManipulator::insertBits(data,(uint32_t)1,60,1));
//         //clr WRITE
//         sequence.pushBack(ChannelFIFOAddress[channel],
//         BitManipulator::insertBits(data,(uint32_t)0,60,1));
//         //clr TALK
//         sequence.pushBack(ChannelFIFOAddress[channel],
//         BitManipulator::insertBits(data,(uint32_t)0,62,1));
//         sequence.pushBack(ChannelFIFOAddress[channel],
//         BitManipulator::insertBits(data,(uint32_t)0x40,56,8));
//         */
//
//        //if(it->first != "SendData" && it->first != "RejectHits") continue;
//        uint32_t data = PSI46DigROCDefinitions::makeDACWriteHeader(
//                            rocStream.getFEWROCAddress(), it->first);
//        //Insert channel
//        BitManipulator::insertBits(data, 1, 16 + channel, 1);
//        sequence.pushBack(ChannelFIFOAddress[channel], it->second.second);
//        std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " << it->first
//        << " value: "
//        << it->second.second << std::hex << " -> Data: " << data << std::dec
//        << std::endl;
//        sequence.pushBack(STRIP_SC_CSR, data);
//    }
//}
//
////==============================================================================
// void PSI46DigFirmware::makeDACBuffer(std::string& buffer,
//                                   unsigned int channel, const ROCStream& rocStream)
//{
//    std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel <<
//    std::endl; const ROCDACs& rocDACs = rocStream.getROCDACs(); for
//    (DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
//    rocDACs.getDACList().end(); it++)
//    {
//        std::string bufferElement;
//        protocolInstance_->waitClear(bufferElement, STRIP_SC_CSR, 0x80000000);
//        uint32_t registerHeader = 0;
//        //FIXME This must go in the PSI46DigROCDefinitions stuff
//        if (it->first != "RejectHits" && it->first != "SendData")
//        {
//            protocolInstance_->write(bufferElement, ChannelFIFOAddress[channel],
//            it->second.second); registerHeader =
//            PSI46DigROCDefinitions::makeDACWriteHeader(
//                                 rocStream.getFEWROCAddress(), it->first);
//            //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
//            it->first << " value: " << it->second.second << hex << " -> Data: " <<
//            registerHeader << dec << std::endl;
//            //Insert channel
//            BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//        }
//        else
//        {
//            if (it->second.second == 1 || it->second.second == 2)
//            {
//                registerHeader = PSI46DigROCDefinitions::makeDACSetHeader(
//                                     rocStream.getFEWROCAddress(), it->first);
//                //Insert channel
//                BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//            }
//            else if (it->second.second == 0 || it->second.second == 5)
//            {
//                registerHeader = PSI46DigROCDefinitions::makeDACResetHeader(
//                                     rocStream.getFEWROCAddress(), it->first);
//                //Insert channel
//                BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//            }
//            else
//                std::cout << __COUT_HDR_FL__ << "Register value for : " << it->first
//                << " doesn't have a value I expect -> value = "
//                << it->second.second << std::endl;
//
//        }
//        //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
//        it->first << " value: " << it->second.second << hex << " -> Data: " <<
//        registerHeader << dec << std::endl; protocolInstance_->write(bufferElement,
//        STRIP_SC_CSR, registerHeader); protocolInstance_->waitClear(bufferElement,
//        STRIP_SC_CSR, 0x80000000);
//
//        //buffer.push_back(bufferElement);
//        buffer += bufferElement;
//        //break;
//    }
//}
//
////==============================================================================
// void PSI46DigFirmware:: makeDACBuffer(std::vector<std::string>& buffer, unsigned int
// channel, const ROCStream& rocStream)
//{
//
//    std::cout << __COUT_HDR_FL__ << "\tMaking DAC Buffer" << std::endl;
//
//    int limitCount = 0;
//    unsigned int singleVectorCount = 0;
//
//    std::string alternateBuffer;
//
//    std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel <<
//    std::endl; const ROCDACs& rocDACs = rocStream.getROCDACs();
//    //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Number of DACs: " <<
//    rocDACs.getDACList().size() << std::endl; std::string bufferElement; for
//    (DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
//    rocDACs.getDACList().end(); it++)
//    {
//        protocolInstance_->waitClear(bufferElement, STRIP_SC_CSR, 0x80000000);
//        uint32_t registerHeader = 0;
//        //FIXME This must go in the PSI46DigROCDefinitions stuff
//        if (it->first != "RejectHits" && it->first != "SendData")
//        {
//            protocolInstance_->write(bufferElement, ChannelFIFOAddress[channel],
//            it->second.second); registerHeader =
//            PSI46DigROCDefinitions::makeDACWriteHeader(
//                                 rocStream.getFEWROCAddress(), it->first);
//            std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
//            it->first << " value: " << it->second.second << std::hex << " -> Data: " <<
//            registerHeader << std::dec << std::endl;
//            //Insert channel
//            BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//        }
//        else
//        {
//            if (it->second.second == 1 || it->second.second == 2)
//            {
//                registerHeader = PSI46DigROCDefinitions::makeDACSetHeader(
//                                     rocStream.getFEWROCAddress(), it->first);
//                //Insert channel
//                BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//            }
//            else if (it->second.second == 0 || it->second.second == 5)
//            {
//                registerHeader = PSI46DigROCDefinitions::makeDACResetHeader(
//                                     rocStream.getFEWROCAddress(), it->first);
//                //Insert channel
//                BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
//            }
//            else
//                std::cout << __COUT_HDR_FL__ << "Register value for : " << it->first
//                << " doesn't have a value I expect -> value = "
//                << it->second.second << std::endl;
//
//        }
//        //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
//        it->first << " value: " << it->second.second << hex << " -> Data: " <<
//        registerHeader << dec << std::endl; protocolInstance_->write(bufferElement,
//        STRIP_SC_CSR, registerHeader); protocolInstance_->waitClear(bufferElement,
//        STRIP_SC_CSR, 0x80000000);
//
//        //alternateBuffer += bufferElement;
//        limitCount++;
//        singleVectorCount++;
//
//        if (limitCount == STIB_DAC_WRITE_MAX)
//        {
//            std::cout << __COUT_HDR_FL__ << "\tBuffer lenght:" << bufferElement.size()
//            << std::endl; buffer.push_back(bufferElement); limitCount = 0;
//            bufferElement.clear();
//        }
//        else if (singleVectorCount == rocDACs.getDACList().size()) //case for imcomplete
//        packet
//        {
//            buffer.push_back(bufferElement);
//        }
//
//        //buffer.push_back(bufferElement);
//        //break;
//    }
//    std::cout << __COUT_HDR_FL__ << "\tDone making DAC Buffer" << std::endl;
//
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskBuffer(std::string& buffer, unsigned int channel,
//                                    const ROCStream& rocStream)
//{
//    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
//    makeMaskBuffer(buffer, channel, rocStream, "Kill");
//    //    makeMaskSequence(buffer, channel, rocStream, "Inject");
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskBuffer(std::string& buffer, unsigned int channel, const
// ROCStream& rocStream, const std::string& registerName)
//{
//    std::cout << __COUT_HDR_FL__ << "\tMaking mask! " << std::endl;
//    int chipId = rocStream.getFEWROCAddress();
//    std::string mask = rocStream.getROCMask();
//    std::cout << __COUT_HDR_FL__ << "\tMask length: " << mask.length() << std::endl;
//
//    unsigned int data[4] = { 0, 0, 0, 0 };
//
//    char val;
//    //int j = 0;
//    for (unsigned int d = 0; d < 4; d++)
//    { //d goes from 0 to 4. 4 bytes
//        //std::cout << __COUT_HDR_FL__ << "---------------------" << d <<
//        "-------------------" << std::endl; for (unsigned int m = 0; m < 8 * 4; m++) {
//        //m goes from 0 to 31, since each byte has 8 bits, there are 32 bits
//            val = mask[(8 * 4 * d) + m]; //assigns to val the value of the corresponding
//            bit. 0-31, 32-63, 64-95, 96-127. it goes through each of the 128 bits
//            //std::cout << __COUT_HDR_FL__ << "---------------------" << j++ <<
//            std::endl;
//            //std::cout << __COUT_HDR_FL__ << "data[" << d << "] before: " << hex <<
//            data[d] << dec << std::endl; data[d] |= (unsigned int) atoi(&val) << (8 * 4
//            - 1 - m);
//            //std::cout << __COUT_HDR_FL__ << "(unsigned int) atoi(&val): " << (unsigned
//            int) atoi(&val) << std::endl;
//            //std::cout << __COUT_HDR_FL__ << "data[" << d << "] after: " << hex <<
//            data[d] << dec << std::endl;
//            //std::cout << __COUT_HDR_FL__ << hex << "D: " << data[d] << " Val: " <<
//            (unsigned int)atoi(&val) << " index: " << dec << (8*4-1-m) << " bit: " <<
//            mask[(8*4*d)+m] << dec << std::endl;
//        }
//        //
//    }
//
//    int i, ierr;
//    unsigned int w;
//    unsigned char len = 4;
//    unsigned char addr = 17;//Kill
//    unsigned char bitMask = 1 << channel;
//    unsigned char inst = WRITE;
//
//    protocolInstance_->waitClear(buffer, STRIP_SC_CSR, 0x80000000);
//
//    for (i = 0; i < 4; i++)
//        //write(buffer, STRIP_SCI + 4 * i, data[i]);
//        protocolInstance_->write(buffer, STRIP_SCI + 4 * (4 - i - 1), data[i]);
//
//    w = 0x80000000 | (len << 24) | (bitMask << 16) | (inst << 10) | (addr << 5) |
//    chipId;
//
//    ierr = protocolInstance_->write(buffer, STRIP_SC_CSR, w);
//
//    protocolInstance_->waitClear(buffer, STRIP_SC_CSR, 0x80000000);
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
//                                      unsigned int channel, const ROCStream& rocStream)
//{
//    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
//    makeMaskSequence(sequence, channel, rocStream, "Kill");
//    //    makeMaskSequence(sequence, channel, rocStream, "Inject");
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
//                                      unsigned int channel, const ROCStream& rocStream)
//{
//    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
//    makeMaskSequence(sequence, channel, rocStream, "Kill");
//    //    makeMaskSequence(channel,rocStream,sequence,"Inject");
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
//                                      unsigned int channel, const ROCStream& rocStream,
//                                      const std::string& registerName)
//{
//    int chipId = rocStream.getFEWROCAddress();//9, 14 or 21 bcast
//    std::string mask = rocStream.getROCMask();
//    std::cout << __COUT_HDR_FL__ << "Mask length: " << mask.length() << std::endl;
//
//    uint64_t uInt64Data = 0;
//    std::string stringData = "";
//
//    //have to manually set every bit for mask writes.
//    //reset fifo
//    //download every bit (shift_in and shift_ctrl) to fifo (setup muxes and control
//    lines)
//    //configure muxes for readout
//    //commence readout
//
//    //FIFO Controls - byte 5 (7:0)
//    //7 - FIFO Clock
//    //6 - Shift Ctrl bit
//    //MASK_CELL_H - Shift In bit
//    //MUX Controls - byte 0 (7:0)
//    //7 - Read En/Output MUX sel (1 for masks)
//    //1 - Write En for mux
//    //0 - Reset Fifo
//
//    //reset fifo
//    BitManipulator::insertBits(uInt64Data, 0x1, 56, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//
//    //configure fifo for write
//    BitManipulator::insertBits(uInt64Data, 0x2, 56, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//
//    //Download every bit (shift_in and shift_control) to fifo (setup muxes and control
//    lines)
//    //Bit 7 of data is FIFO clock, bit 6 is shift_control, bit 5 is shift_in
//    //start bits (ctrl 0, data 0 => ctrl 1, data 0)
//
//    BitManipulator::insertBits(uInt64Data, 0x40, 16, 8);//(0<<7) | (1<<6) | (0<<5)
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//    BitManipulator::insertBits(uInt64Data, 0xc0, 16, 8);//(1<<7) | (1<<6) | (0<<5) ->
//    clock the data in the fifo sequence.pushBack(ChannelFIFOAddress[channel],
//    uInt64Data);
//
//    stringData = PSI46DigROCDefinitions::makeMaskWriteCommand(chipId,
//                 registerName, mask);
//
//    uint8_t data;
//    for (unsigned int s = 0; s < stringData.length(); s++)
//        for (int b = 8 - 1; b >= 0 && (s * 8 + 8 - b < 13 + 128); b--)
//        {
//            //std::cout << __COUT_HDR_FL__ << "S: " << s << " val: " <<
//            std::stringData.data()[s] << " b: " << b << " bit: " <<
//            ((stringData.data()[s]>>b)&1) << std::endl; data = 0x40 |
//            (((stringData.data()[s] >> b) & 1) << 5);
//            BitManipulator::insertBits(uInt64Data, (uint64_t) data, 16, 8);
//            sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//            data |= 0x80;
//            BitManipulator::insertBits(uInt64Data, (uint64_t) data, 16, 8);
//            sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//        }
//
//    //reset Shift Control
//    BitManipulator::insertBits(uInt64Data, 0x0, 16, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//    BitManipulator::insertBits(uInt64Data, 0x80, 16, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//
//    //configure muxes for readout
//    BitManipulator::insertBits(uInt64Data, 0x0, 56, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//
//    //commence readout
//    BitManipulator::insertBits(uInt64Data, 0x80, 56, 8);
//    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
//
//}
//
////==============================================================================
// void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
//                                      unsigned int channel, const ROCStream& rocStream,
//                                      const std::string& registerName)
//{}
//
////==============================================================================
// std::string PSI46DigFirmware::readCSRRegister()
//{
//    std::string buffer;
//    read(buffer,STRIP_CSR);
//    return buffer;
//}
//
////==============================================================================
// void PSI46DigFirmware::setFrequencyFromClockState(std::string& buffer, double
// frequency)
//{
//    std::cout << __COUT_HDR_FL__ << "Setting up clock frequency!!!" << std::endl;
//
//    int quotient;
//    int numerator;
//    int denominator;
//    double realClockFrequency;
//
//    if (BitManipulator::readBits(stripCSRRegisterValue_, 17, 1) == 1) //if fastBCO is
//    enabled
//        quotient = 4;
//    else //normal cases
//        quotient = 8;
//
//    if (isClockStateExternal()) //base freq: 54MHz
//    {
//        realClockFrequency = EXTERNAL_CLOCK_FREQUENCY / quotient; //this is the REAL
//        frequency being used
//    }
//    else //base freq: 66.667MHz
//    {
//        realClockFrequency = INTERNAL_CLOCK_FREQUENCY / quotient; //this is the REAL
//        frequency being used
//    }
//
//    double factor = frequency / realClockFrequency;
//
//    //The device needs the denominator and the denominator to be load into a 5 bit
//    register
//    //It will take two initial numerator and denominator bigger than necessary (to do
//    not loose precision)
//    //and divide them for their gcd. If they still do not fit in 5 bit, they are trunked
//    (divided by 2)
//    //untill they are less than 32
//    numerator = factor * 100; //we will work with 2 digits precision after the decimal
//    point denominator = 100;
//
//    do
//    {
//        //We will need the GCD at some point in order to simplify fractions //taken from
//        other sources int gcd = numerator; int rest = denominator; int tmp;
//
//        while (rest != 0)
//        {
//            tmp = rest;
//            rest = gcd % tmp;
//            gcd = tmp;
//        }
//        //Done finding the GCD
//
//        if (gcd == 1) //if there's no GCD, just divide by 2 to find the nearest
//        approssimation with less bits
//        {
//            numerator /= 2;
//            denominator /= 2;
//        }
//        else
//        {
//            numerator /= gcd;
//            denominator /= gcd;
//        }
//
//    }
//    while (denominator >= 32 || numerator >= 32);
//    std::cout << __COUT_HDR_FL__ << "Numerator: " << numerator << std::endl;
//    std::cout << __COUT_HDR_FL__ << "Denominator: " << denominator << std::endl;
//    setFrequencyRatio(buffer, numerator, denominator);
//    std::cout << __COUT_HDR_FL__ << "Done with clock frequency setup!!!" << std::endl;
//}
////==============================================================================
// bool PSI46DigFirmware::isClockStateExternal() //returns true if the clock state is
// External
//{
//    if (BitManipulator::readBits(stripCSRRegisterValue_, 16, 1) == 1)
//        return true;
//    else
//        return false;
//}
//
////==============================================================================
// void PSI46DigFirmware::setCSRRegister(uint32_t total)
//{
//    stripCSRRegisterValue_ = total;
//}
//
////==============================================================================
// void PSI46DigFirmware::setPacketSizeStripCSR(uint32_t size)
//{
//    if (size > 31)
//        std::cout << __COUT_HDR_FL__
//        << "ERROR: Maximum packet size is 31 while here you are trying to set "
//        << size << " packets!" << std::endl;
//    BitManipulator::insertBits(stripCSRRegisterValue_, size, 3, 5);
//    //write(buffer,STRIP_CSR, stripSCRRegisterValue_);
//}
//
////==============================================================================
// void PSI46DigFirmware::enableChannelsStripCSR(bool channel0, bool channel1,
//        bool channel2, bool channel3, bool channel4, bool channel5)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, ((uint32_t) channel0)
//                                 + ((uint32_t) channel1 << 1) + ((uint32_t) channel2 <<
//                                 2)
//                                 + ((uint32_t) channel3 << 3) + ((uint32_t) channel4 <<
//                                 4)
//                                 + ((uint32_t) channel5 << 5), 8, 6);
//}
//
////==============================================================================
// void PSI46DigFirmware::setExternalBCOClockSourceStripCSR(std::string clockSource)
//{
//    if (clockSource == "External")
//        BitManipulator::insertBits(stripCSRRegisterValue_, 1, 16, 1);
//    else if (clockSource == "Internal")
//        BitManipulator::insertBits(stripCSRRegisterValue_, 0, 16, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::setHaltStripCSR(bool set)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) set, 17, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::enableBCOStripCSR(bool enable)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 19, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::flushBuffersStripCSR(void)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 20, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::resetTriggerCounterStripCSR(std::string& buffer)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//
//    BitManipulator::insertBits(stripCSRRegisterValue_, 0, 21, 1);
//    protocolInstance_->write(buffer, STRIP_CSR, stripCSRRegisterValue_);
//}
//
////==============================================================================
// void PSI46DigFirmware::resetBCOCounterStripCSR(void)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 22, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::enableTriggerStripCSR(bool enable)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 23, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::sendTriggerDataStripCSR(bool send)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 24, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::sendTriggerNumberStripCSR(bool send)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 25, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::sendBCOStripCSR(bool send)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 26, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::enableStreamStripCSR(bool enable)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 27, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::resetDCMStripCSR(bool clear)
//{
//    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) clear, 31, 1);
//}
//
////==============================================================================
// uint32_t PSI46DigFirmware::waitDCMResetStripCSR(void)
//{
//    uint32_t bitToCheck = 0;
//    BitManipulator::insertBits(bitToCheck, 1, 31, 2);
//    return bitToCheck;
//}
//
////==============================================================================
// void PSI46DigFirmware::resetDAC(void)
//{
//    BitManipulator::insertBits(stripResetRegisterValue_, 1, 27, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::resetLink(bool channel0, bool channel1, bool channel2,
//                               bool channel3, bool channel4, bool channel5)
//{
//    stripResetRegisterValue_ = 0;
//    BitManipulator::insertBits(stripResetRegisterValue_,
//                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
//                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3 <<
//                                 3)
//                                 + ((uint32_t) channel4 << 4) + ((uint32_t) channel5 <<
//                                 5), 0, 6);
//    BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1);
//    /*
//     write(buffer, STRIP_RESET, (1<<28) +
//     ((uint32_t)channel0) +
//     ((uint32_t)channel1<<1) +
//     ((uint32_t)channel2<<2) +
//     ((uint32_t)channel3<<3) +
//     ((uint32_t)channel4<<4) +
//     ((uint32_t)channel5<<5)
//     );
//     */
//}
//
////==============================================================================
// void PSI46DigFirmware::clearErrors(bool channel0, bool channel1, bool channel2,
//                                 bool channel3, bool channel4, bool channel5)
//{
//    stripResetRegisterValue_ = 0;
//    BitManipulator::insertBits(stripResetRegisterValue_,
//                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
//                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3 <<
//                                 3)
//                                 + ((uint32_t) channel4 << 4) + ((uint32_t) channel5 <<
//                                 5), 0, 6);
//    BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::clearFIFO(bool channel0, bool channel1, bool channel2,
//                               bool channel3, bool channel4, bool channel5)
//{
//    stripResetRegisterValue_ = 0;
//    BitManipulator::insertBits(stripResetRegisterValue_,
//                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
//                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3 <<
//                                 3)
//                                 + ((uint32_t) channel4 << 4) + ((uint32_t) channel5 <<
//                                 5), 0, 6);
//    BitManipulator::insertBits(stripResetRegisterValue_, 1, 30, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::resetChip(bool channel0, bool channel1, bool channel2,
//                               bool channel3, bool channel4, bool channel5)
//{
//    stripResetRegisterValue_ = 0;
//    BitManipulator::insertBits(stripResetRegisterValue_,
//                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
//                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3 <<
//                                 3)
//                                 + ((uint32_t) channel4 << 4) + ((uint32_t) channel5 <<
//                                 5), 0, 6);
//    BitManipulator::insertBits(stripResetRegisterValue_, 1, 31, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::setFrequencyRatio(std::string& buffer, int numerator, int
// denominator)
//{
//    //The device need to load numerator minus one and denominator minus one, with an
//    internal address of 0x50 and 052 respectively protocolInstance_->write(buffer,
//    STRIP_BCO_DCM, 0x80500000 + (numerator - 1)); //  Set BCOCLK numerator // was
//    0x80500003 protocolInstance_->waitClear(buffer, STRIP_BCO_DCM, 0xf0000000); //  Wait
//    DCM write to finish // was 0x80000000
//
//    protocolInstance_->write(buffer, STRIP_BCO_DCM, 0x80520000 + (denominator - 1)); //
//    Set BCOCLK denominator // was 0x80520001 protocolInstance_->waitClear(buffer,
//    STRIP_BCO_DCM, 0xf0000000); //  Wait DCM write to finish - BCO frequency should
//    be 13.513 MHz // was 0x80000000
//}
//
////==============================================================================
// void PSI46DigFirmware::BCOOffset(uint32_t offset)
//{
//    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, offset, 0, 4);
//}
//
////==============================================================================
// void PSI46DigFirmware::selectSpyFIFO(uint32_t input)
//{
//    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, input, 4, 3);
//}
//
////==============================================================================
// void PSI46DigFirmware::halt(bool halt)
//{
//    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, (uint32_t) halt, 7, 1);
//}
//
////==============================================================================
// void PSI46DigFirmware::configureStripTriggerUnbiased(std::string& buffer)
//{
//    protocolInstance_->write(buffer, STRIP_TRIG_UNBIASED, 0x002805c); //  Configure
//    unbiased trigger
//}
//
////==============================================================================
// void PSI46DigFirmware::configureTriggerInputs(std::string& buffer)
//{
//    protocolInstance_->write(buffer, STRIP_TRIG_INPUT_0, 0x3f440000); //  PSI46Dig
//    GOTHIT trigger input channel 0,1 protocolInstance_->write(buffer,
//    STRIP_TRIG_INPUT_1, 0x3f440000); //  PSI46Dig GOTHIT trigger input channel 2,3
//    protocolInstance_->write(buffer, STRIP_TRIG_INPUT_2, 0x0); //  PSI46Dig GOTHIT
//    trigger input channel 4,5
//}
//
///*
// //==============================================================================
// void PSI46DigFirmware::chipID(uint32_t size)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, size, 0, 5);
// }
//
// //==============================================================================
// void PSI46DigFirmware::addressSlowControls(uint32_t size)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, size, 5, 5);
// }
//
// //==============================================================================
// void PSI46DigFirmware::instructionSlowControls(uint32_t size)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, size, 10, 3);
// }
//
// //==============================================================================
// void PSI46DigFirmware::channelreadSelect(uint32_t size)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, size, 13, 3);
// }
//
// //==============================================================================
// void PSI46DigFirmware::channelMask(uint32_t size)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, size, 16, 8);
// }
//
// //==============================================================================
// void PSI46DigFirmware::bitsLength(uint32_t length)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, length, 26, 3);
// }
//
//
// //==============================================================================
// void PSI46DigFirmware::syncFallingBCO(bool sync)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)sync, 28, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::syncRisingBCO(bool sync)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)sync, 29, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::setRaw(bool set)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)set, 30, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::initSlowControls(bool init)
// {
// BitManipulator::insertBits(stripSCRegisterValue_, (uint32_t)init, 31, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::resetCount(bool reset)
// {
// BitManipulator::insertBits(stripAnalysisCSRRegisterValue_, (uint32_t)reset, 30, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::setBCO_0(uint32_t void PSI46DigFirmware::BCOOffset(uint32_t
// offset)
// {
// BitManipulator::insertBits(stripTrigCSRRegisterValue_, offset, 0, 4);
// }input)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, input, 0, 8);
// }
//
// //==============================================================================
// void PSI46DigFirmware::setBCO_1(uint32_t input)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, input, 8, 8);
// }
//
// //==============================================================================
// void PSI46DigFirmware::trimFracBCO_0(uint32_t input)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, input, 16, 4);
// }
//
// //==============================================================================
// void PSI46DigFirmware::trimFracBCO_1(uint32_t input)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, input, 20, 4);
// }
//
// //==============================================================================
// void PSI46DigFirmware::enable_0(bool enable)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, (uint32_t)enable, 28, 1);
// }
//
// //==============================================================================
// void PSI46DigFirmware::enable_1(bool enable)
// {
// BitManipulator::insertBits(trigInputRegisterValue_, (uint32_t)enable, 29, 1);
// }
//
// */
//
//
//
//
////#include "otsdaq/DAQHardware/PSI46DigFirmware.h"
////#include "otsdaq/DAQHardware/PSI46DigFirmwareDefinitions.h"
////#include "otsdaq/BitManipulator/BitManipulator.h"
////#include "otsdaq/DetectorHardware/PSI46DigROCDefinitions.h"
////#include "otsdaq/MessageFacility/MessageFacility.h"
//#include "otsdaq/Macros/CoutMacros.h"
////#include "otsdaq/ConfigurationDataFormats/OtsUDPFEWTable.h"
////
////#include <sys/socket.h>
////#include <netinet/in.h>
////#include <arpa/inet.h>
////
////#include <iostream>
////#include <cstdlib>
////
////
////
////
//////==============================================================================
////PSI46DigFirmware::PSI46DigFirmware(unsigned int version) :
////        FirmwareBase(version),
////        stripCSRRegisterValue_(0)
////{}
////
//////==============================================================================
////PSI46DigFirmware::~PSI46DigFirmware(void)
////{}
////
//////==============================================================================
////int PSI46DigFirmware::init(ConfigurationManager *configManager)
////{
////    return 0;
////}
////
//////==============================================================================
////string PSI46DigFirmware::configureClocks(std::string source, double frequency)
////{
////    std::cout << __COUT_HDR_FL__ << "Writing Clock configuration!" << std::endl;
////
////    std::string buffer;
////    //NoNeedNowwrite(buffer, ETHIO_DESTINATION_PORT, 0x0000b798); //  Listen port for
/// ethio stuff
////
////    setPacketSizeStripCSR(6);
////    setExternalBCOClockSourceStripCSR(source); //(source)
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Reset CSR - reset trigger
/// counter, external 27 MHz clock
////
////    resetDCMStripCSR(true);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Set reset to DCM
////
////    resetDCMStripCSR(false);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_); //  Clear reset to DCM
////
////    ;
////    waitClear(buffer, STRIP_CSR, waitDCMResetStripCSR()); //  Wait for DCM to lock
////
////    write(buffer, STRIP_TRIM_CSR, 0x00002000); //  MCLKB edge for channel 5 // was
/// 0x00002000
////
////    setFrequencyFromClockState(buffer, frequency);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    resetDCMStripCSR(true);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    resetDCMStripCSR(false);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    waitClear(buffer, STRIP_CSR, waitDCMResetStripCSR()); //  Wait for DCM to lock
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue :" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl;
////
////    return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::resetDetector(int channel)
////{
////    std::cout << __COUT_HDR_FL__ << "Resetting detector!" << std::endl;
////    std::string buffer;
////    if (channel == -1)//reset all channels
////    {
////        //write(buffer,STRIP_RESET,0xd000003f);                  //  Issue reset
////        write(buffer, STRIP_RESET, 0xf000003f); //  Issue reset // was 0xf000003f
////        waitClear(buffer, STRIP_RESET, 0xf0000000); //  Wait for reset to complete //
/// was 0xf0000000 /    } else /    { /        write(buffer, STRIP_RESET, 0xf000003f); //
/// Issue reset /        waitClear(buffer, STRIP_RESET, 0xf0000000); //  Wait for reset to
/// complete /    }
////
////    return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::enableTrigger(void)
////{
////    std::string buffer;
////    std::cout << __COUT_HDR_FL__ << "Enabling Trigger!!!" << std::endl;
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in :" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl;
////
////    setHaltStripCSR(1);//WARNING THIS IS CLEARED BY THE MASTER BUT IF THERE IS NO
/// MASTER NOTHING WORKS UNLESS THE BIT IS UNSET /    //setHaltStripCSR(0);//WARNING THIS
/// IS CLEARED BY THE MASTER BUT IF THERE IS NO MASTER NOTHING WORKS UNLESS THE BIT IS
/// UNSET /    sendTriggerDataStripCSR(true); /    sendTriggerNumberStripCSR(true); /
/// sendBCOStripCSR(true); /    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    stripTriggerCSRRegisterValue_ = 0;
////    BCOOffset(4);
////    write(buffer, STRIP_TRIG_CSR, stripTriggerCSRRegisterValue_); //  BCO offset //
/// was 0x00000004
////
////    //  write(buffer,STRIP_TRIG_INPUT_0,0x1f060040);  //  PSI46Dig GOTHIT trigger
/// input - timed in for the 27 MHz external clock /    //
/// write(buffer,STRIP_TRIG_INPUT_3,0x3f874040);  //  Unbiased trigger input + external
/// trigger
////
////    configureStripTriggerUnbiased(buffer);
////
////    configureTriggerInputs(buffer);
////
////    //FIXME for IP .36 the number to set is 0x20401000
////
////    if (version_ == 1)
////        write(buffer, STRIP_TRIG_INPUT_3, 0x20401000); // Turn on streaming hits along
/// with BCO data /    else if (version_ == 2) /        write(buffer, STRIP_TRIG_INPUT_3,
/// 0x20301000); // Turn on streaming hits along with BCO data /    else /    { /
/// std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "what version is this?" <<
/// version_ << std::endl; /	assert(0); /    } /    std::cout << __COUT_HDR_FL__ <<
///"stripCSRRegisterValue out:" << hex << stripCSRRegisterValue_ << dec << std::endl; /
/// std::cout << __COUT_HDR_FL__ << "Done enabling Trigger!!!" << std::endl;
////
////    return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::setDataDestination(std::string ip, unsigned int port)
////{
////    std::cout << __COUT_HDR_FL__ << "Set data destination!" << std::endl;
////    std::string buffer;
////    struct sockaddr_in socketAddress;
////    inet_pton(AF_INET, ip.c_str(), &(socketAddress.sin_addr));
////    //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "ADDRESS: " << hex <<
/// ntohl(socketAddress.sin_addr.s_addr) << dec << std::endl; /    write(buffer,
/// DATA_DESTINATION_IP, ntohl(socketAddress.sin_addr.s_addr)); //  Set data destination
/// IP  192.168.133.1 /    //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "PORT:
/// "
/// <<  hex << 0xbeef0000+port << dec << std::endl; /    write(buffer,
/// DATA_SOURCE_DESTINATION_PORT, 0xbeef0000 + port); //  Set data destination port /
/// return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::resetBCO(void)
////{
////    std::cout << __COUT_HDR_FL__ << "Reset BCO!!!" << std::endl;
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in :" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl; /    std::string buffer;
////
////    resetTriggerCounterStripCSR(buffer);
////    //write(buffer, STRIP_CSR, stripCSRRegisterValue_);//the write is done in the
/// reset /    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue :" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl;
////
////    //msg->Write(STRIP_SC_CSR,0x90000b95|(chmask<<16));
////    //write(buffer,STRIP_SC_CSR,0x900f0b95);//  This is the <SCR,set> command with the
/// bit set to sync SHIFT with BCO=0.
////
////
////    enableBCOStripCSR(true);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue out:" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl; /    std::cout << __COUT_HDR_FL__ << "Done
/// reset BCO!!!" << std::endl;
////
////    return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::startStream(bool channel0, bool channel1, bool channel2,
/// bool channel3, bool channel4, bool channel5)
////{
////    std::cout << __COUT_HDR_FL__ << "Start Stream!" << std::endl;
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue in:" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl; /    std::string buffer;
////
////    std::cout << __COUT_HDR_FL__ << " channel0 " << channel0 << " channel1 " <<
/// channel1 << " channel2 " << channel2 << " channel3 " << channel3 << " channel4 " <<
/// channel4 << " channel5 " << channel5 << std::endl;
////
////    enableChannelsStripCSR(channel0, channel1, channel2, channel3, channel4,
/// channel5); /    //    if (version_ == 1) /    //        enableChannelsStripCSR(true,
/// true, true, true, false, false); /    //    else if (version_ == 2) /    //
/// enableChannelsStripCSR(true, true, true, true, true, true);
////
////    enableStreamStripCSR(true); //  Turn on streaming hits along with BCO data // was
/// 0x0f000f30 /    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    std::cout << __COUT_HDR_FL__ << "stripCSRRegisterValue out:" << hex <<
/// stripCSRRegisterValue_ << dec << std::endl; /    std::cout << __COUT_HDR_FL__ << "Done
/// start Stream!" << std::endl;
////
////    return buffer;
////}
////
//////==============================================================================
////std::string PSI46DigFirmware::stopStream(void)
////{
////    std::string buffer;
////    enableChannelsStripCSR(false, false, false, false, false, false);
////    enableStreamStripCSR(false);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////    return buffer;
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeDACSequence(FirmwareSequence<uint64_t>& sequence,
////                                     unsigned int channel, const ROCStream& rocStream)
////{
////    const ROCDACs& rocDACs = rocStream.getROCDACs();
////    for (DACList::const_iterator it = rocDACs.getDACList().begin(); it
////            != rocDACs.getDACList().end(); it++)
////    {
////        //if(it->first != "SendData" && it->first != "RejectHits") continue;
////        uint64_t data = PSI46DigROCDefinitions::makeDACWriteCommand(
////                            rocStream.getFEWROCAddress(), it->first,
/// it->second.second); /        sequence.pushBack(ChannelFIFOAddress[channel], data); /
/// sequence.pushBack(ChannelFIFOAddress[channel], /
/// BitManipulator::insertBits(data, (uint64_t) 0x48, 56, 8)); /        //set WRITE /
/// sequence.pushBack(ChannelFIFOAddress[channel], /
/// BitManipulator::insertBits(data, (uint64_t) 1, 60, 1)); /        //clr WRITE /
/// sequence.pushBack(ChannelFIFOAddress[channel], /
/// BitManipulator::insertBits(data, (uint64_t) 0, 60, 1)); /        //clr TALK /
/// sequence.pushBack(ChannelFIFOAddress[channel], /
/// BitManipulator::insertBits(data, (uint64_t) 0, 62, 1)); /
/// sequence.pushBack(ChannelFIFOAddress[channel], /
/// BitManipulator::insertBits(data, (uint64_t) 0x40, 56, 8)); /        //break; /    }
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeDACSequence(FirmwareSequence<uint32_t>& sequence,
////                                     unsigned int channel, const ROCStream& rocStream)
////{
////    const ROCDACs& rocDACs = rocStream.getROCDACs();
////    for (DACList::const_iterator it = rocDACs.getDACList().begin(); it
////            != rocDACs.getDACList().end(); it++)
////    {
////        /*RYAN
////         //if(it->first != "SendData" && it->first != "RejectHits") continue;
////         uint64_t data =
/// PSI46DigROCDefinitions::makeDACWriteCommand(rocStream.getFEWROCAddress(), it->first,
/// it->second.second); /         sequence.pushBack(ChannelFIFOAddress[channel], data); /
/// sequence.pushBack(ChannelFIFOAddress[channel],
/// BitManipulator::insertBits(data,(uint32_t)0x48,56,8)); /         //set WRITE /
/// sequence.pushBack(ChannelFIFOAddress[channel],
/// BitManipulator::insertBits(data,(uint32_t)1,60,1)); /         //clr WRITE /
/// sequence.pushBack(ChannelFIFOAddress[channel],
/// BitManipulator::insertBits(data,(uint32_t)0,60,1)); /         //clr TALK /
/// sequence.pushBack(ChannelFIFOAddress[channel],
/// BitManipulator::insertBits(data,(uint32_t)0,62,1)); /
/// sequence.pushBack(ChannelFIFOAddress[channel],
/// BitManipulator::insertBits(data,(uint32_t)0x40,56,8)); /         */
////
////        //if(it->first != "SendData" && it->first != "RejectHits") continue;
////        uint32_t data = PSI46DigROCDefinitions::makeDACWriteHeader(
////                            rocStream.getFEWROCAddress(), it->first);
////        //Insert channel
////        BitManipulator::insertBits(data, 1, 16 + channel, 1);
////        sequence.pushBack(ChannelFIFOAddress[channel], it->second.second);
////        std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
/// it->first << " value: " /        << it->second.second << hex << " -> Data: " << data
/// <<  dec /        << std::endl; /        sequence.pushBack(STRIP_SC_CSR, data); /    }
////}
////
//////==============================================================================
/////*
////void PSI46DigFirmware::makeDACBuffer(std::string& buffer, unsigned int channel,
////		const ROCStream& rocStream) {
////	const ROCDACs& rocDACs = rocStream.getROCDACs();
////	for (DACList::const_iterator it = rocDACs.getDACList().begin(); it
////			!= rocDACs.getDACList().end(); it++) {
////		waitClear(buffer, STRIP_SC_CSR, 0x80000000);
////		write(buffer, ChannelFIFOAddress[channel], it->second.second);
////		uint32_t registerHeader = PSI46DigROCDefinitions::makeDACWriteHeader(
////				rocStream.getFEWROCAddress(), it->first);
////		//std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " <<
/// channel
///<< "  Register: " << it->first << " value: " << it->second.second << hex << " -> Data:
///" <<
/// registerHeader << dec << std::endl; /		//Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /		std::cout <<
///__COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel << " Register: " /
///<< it->first << " value: " << it->second.second << hex /				<< " -> Data: " <<
/// registerHeader << dec << std::endl; /		write(buffer, STRIP_SC_CSR,
/// registerHeader); /		waitClear(buffer, STRIP_SC_CSR, 0x80000000); /		//break;
////	}
////}
////*/
////
//////==============================================================================
////void PSI46DigFirmware::makeDACBuffer(std::string& buffer,
////                                   unsigned int channel, const ROCStream& rocStream)
////{
////    std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel <<
/// std::endl; /    const ROCDACs& rocDACs = rocStream.getROCDACs(); /    for
///(DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
/// rocDACs.getDACList().end(); it++) /    { /        std::string bufferElement; /
/// waitClear(bufferElement, STRIP_SC_CSR, 0x80000000); /        uint32_t registerHeader =
/// 0; /        //FIXME This must go in the PSI46DigROCDefinitions stuff /        if
///(it->first != "RejectHits" && it->first != "SendData") /        { /
/// write(bufferElement, ChannelFIFOAddress[channel], it->second.second); /
/// registerHeader = PSI46DigROCDefinitions::makeDACWriteHeader( /
/// rocStream.getFEWROCAddress(), it->first); /            //std::cout << __COUT_HDR_FL__
///<< __PRETTY_FUNCTION__ << "Register: " << it->first << " value: " << it->second.second
///<< hex << " -> Data: " << registerHeader << dec << std::endl; /            //Insert
/// channel /            BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /
///} /        else /        { /            if (it->second.second == 1 || it->second.second
///== 2) /            { /                registerHeader =
/// PSI46DigROCDefinitions::makeDACSetHeader( /
/// rocStream.getFEWROCAddress(), it->first); /                //Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /            } /
/// else if (it->second.second == 0 || it->second.second == 5) /            { /
/// registerHeader = PSI46DigROCDefinitions::makeDACResetHeader( /
/// rocStream.getFEWROCAddress(), it->first); /                //Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /            } /
/// else /                std::cout << __COUT_HDR_FL__ << "Register value for : " <<
/// it->first /                << " doesn't have a value I expect -> value = " /
///<< it->second.second << std::endl;
////
////        }
////        //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
/// it->first << " value: " << it->second.second << hex << " -> Data: " << registerHeader
///<< dec << std::endl; /        write(bufferElement, STRIP_SC_CSR, registerHeader); /
/// waitClear(bufferElement, STRIP_SC_CSR, 0x80000000);
////
////        //buffer.push_back(bufferElement);
////        buffer += bufferElement;
////        //break;
////    }
////}
////
//////==============================================================================
/////*
//// * //Clifford... Replaced with other... SEE FOLLOWING FUNCTION BELOW
////void PSI46DigFirmware::makeDACBuffer(std::vector<std::string>& buffer,
////		unsigned int channel, const ROCStream& rocStream) {
////	std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel <<
/// std::endl; /	const ROCDACs& rocDACs = rocStream.getROCDACs(); /	for
///(DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
/// rocDACs.getDACList().end(); it++) { /	std::string bufferElement; /
/// waitClear(bufferElement, STRIP_SC_CSR, 0x80000000); /		uint32_t registerHeader =
/// 0; /		//FIXME This must go in the PSI46DigROCDefinitions stuff /		if
/// (it->first
///!= "RejectHits" && it->first != "SendData") { /			write(bufferElement,
/// ChannelFIFOAddress[channel], it->second.second); /			registerHeader =
/// PSI46DigROCDefinitions::makeDACWriteHeader(
////					rocStream.getFEWROCAddress(), it->first);
////			//std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
/// it->first
///<< " value: " << it->second.second << hex << " -> Data: " << registerHeader << dec <<
/// std::endl; /			//Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /		} else { /
/// if (it->second.second == 1 || it->second.second == 2) {
////				registerHeader = PSI46DigROCDefinitions::makeDACSetHeader(
////						rocStream.getFEWROCAddress(), it->first);
////				//Insert channel
////				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
////			} else if (it->second.second == 0 || it->second.second == 5) {
////				registerHeader = PSI46DigROCDefinitions::makeDACResetHeader(
////						rocStream.getFEWROCAddress(), it->first);
////				//Insert channel
////				BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1);
////			} else
////				std::cout << __COUT_HDR_FL__ << "Register value for : " << it->first
////						<< " doesn't have a value I expect -> value = "
////						<< it->second.second << std::endl;
////
////		}
////		//std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
/// it->first
///<< " value: " << it->second.second << hex << " -> Data: " << registerHeader << dec <<
/// std::endl; /		write(bufferElement, STRIP_SC_CSR, registerHeader); /
/// waitClear(bufferElement, STRIP_SC_CSR, 0x80000000);
////
////		buffer.push_back(bufferElement);
////		//break;
////	}
////}
////*/
////
//////==============================================================================
////void PSI46DigFirmware:: makeDACBuffer(std::vector<std::string>& buffer, unsigned int
/// channel, const ROCStream& rocStream)
////{
////
////    std::cout << __COUT_HDR_FL__ << "\tMaking DAC Buffer" << std::endl;
////
////    int limitCount = 0;
////    unsigned int singleVectorCount = 0;
////
////    std::string alternateBuffer;
////
////    std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Channel: " << channel <<
/// std::endl; /    const ROCDACs& rocDACs = rocStream.getROCDACs(); /    //std::cout <<
///__COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Number of DACs: " <<
/// rocDACs.getDACList().size() << std::endl; /    std::string bufferElement; /    for
///(DACList::const_iterator it = rocDACs.getDACList().begin(); it !=
/// rocDACs.getDACList().end(); it++) /    { /        waitClear(bufferElement,
/// STRIP_SC_CSR, 0x80000000); /        uint32_t registerHeader = 0; /        //FIXME This
/// must go in the PSI46DigROCDefinitions stuff /        if (it->first != "RejectHits" &&
/// it->first != "SendData") /        { /            write(bufferElement,
/// ChannelFIFOAddress[channel], it->second.second); /            registerHeader =
/// PSI46DigROCDefinitions::makeDACWriteHeader( /
/// rocStream.getFEWROCAddress(), it->first); /            std::cout << __COUT_HDR_FL__ <<
///__PRETTY_FUNCTION__ << "Register: " << it->first << " value: " << it->second.second <<
/// hex << " -> Data: " << registerHeader << dec << std::endl; /            //Insert
/// channel /            BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /
///} /        else /        { /            if (it->second.second == 1 || it->second.second
///== 2) /            { /                registerHeader =
/// PSI46DigROCDefinitions::makeDACSetHeader( /
/// rocStream.getFEWROCAddress(), it->first); /                //Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /            } /
/// else if (it->second.second == 0 || it->second.second == 5) /            { /
/// registerHeader = PSI46DigROCDefinitions::makeDACResetHeader( /
/// rocStream.getFEWROCAddress(), it->first); /                //Insert channel /
/// BitManipulator::insertBits(registerHeader, 1, 16 + channel, 1); /            } /
/// else /                std::cout << __COUT_HDR_FL__ << "Register value for : " <<
/// it->first /                << " doesn't have a value I expect -> value = " /
///<< it->second.second << std::endl;
////
////        }
////        //std::cout << __COUT_HDR_FL__ << __PRETTY_FUNCTION__ << "Register: " <<
/// it->first << " value: " << it->second.second << hex << " -> Data: " << registerHeader
///<< dec << std::endl; /        write(bufferElement, STRIP_SC_CSR, registerHeader); /
/// waitClear(bufferElement, STRIP_SC_CSR, 0x80000000);
////
////        //alternateBuffer += bufferElement;
////        limitCount++;
////        singleVectorCount++;
////
////        if (limitCount == STIB_DAC_WRITE_MAX)
////        {
////            std::cout << __COUT_HDR_FL__ << "\tBuffer lenght:" << bufferElement.size()
///<< std::endl; /            buffer.push_back(bufferElement); /            limitCount =
/// 0; /            bufferElement.clear(); /        } /        else if (singleVectorCount
///== rocDACs.getDACList().size()) //case for imcomplete packet /        { /
/// buffer.push_back(bufferElement); /        }
////
////        //buffer.push_back(bufferElement);
////        //break;
////    }
////    std::cout << __COUT_HDR_FL__ << "\tDone making DAC Buffer" << std::endl;
////
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskBuffer(std::string& buffer, unsigned int channel,
////                                    const ROCStream& rocStream)
////{
////    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
////    makeMaskBuffer(buffer, channel, rocStream, "Kill");
////    //    makeMaskSequence(buffer, channel, rocStream, "Inject");
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskBuffer(std::string& buffer, unsigned int channel, const
/// ROCStream& rocStream, const std::string& registerName)
////{
////    std::cout << __COUT_HDR_FL__ << "\tMaking mask! " << std::endl;
////    int chipId = rocStream.getFEWROCAddress();
////    std::string mask = rocStream.getROCMask();
////    std::cout << __COUT_HDR_FL__ << "\tMask length: " << mask.length() << std::endl;
////
////    unsigned int data[4] = { 0, 0, 0, 0 };
////
////    char val;
////    //int j = 0;
////    for (unsigned int d = 0; d < 4; d++)
////    { //d goes from 0 to 4. 4 bytes
////        //std::cout << __COUT_HDR_FL__ << "---------------------" << d <<
///"-------------------" << std::endl; /        for (unsigned int m = 0; m < 8 * 4; m++)
////        { //m goes from 0 to 31, since each byte has 8 bits, there are 32 bits
////            val = mask[(8 * 4 * d) + m]; //assigns to val the value of the
/// corresponding bit. 0-31, 32-63, 64-95, 96-127. it goes through each of the 128 bits /
/////std::cout << __COUT_HDR_FL__ << "---------------------" << j++ << std::endl; /
/////std::cout << __COUT_HDR_FL__ << "data[" << d << "] before: " << hex << data[d] << dec
///<< std::endl; /            data[d] |= (unsigned int) atoi(&val) << (8 * 4 - 1 - m); /
/////std::cout << __COUT_HDR_FL__ << "(unsigned int) atoi(&val): " << (unsigned int)
/// atoi(&val) << std::endl; /            //std::cout << __COUT_HDR_FL__ << "data[" << d
/// <<
///"] after: " << hex << data[d] << dec << std::endl; /            //std::cout <<
///__COUT_HDR_FL__ << hex << "D: " << data[d] << " Val: " << (unsigned int)atoi(&val) << "
/// index: " << dec << (8*4-1-m) << " bit: " << mask[(8*4*d)+m] << dec << std::endl; /
///} /        // /    }
////
////    int i, ierr;
////    unsigned int w;
////    unsigned char len = 4;
////    unsigned char addr = 17;//Kill
////    unsigned char bitMask = 1 << channel;
////    unsigned char inst = WRITE;
////
////    waitClear(buffer, STRIP_SC_CSR, 0x80000000);
////
////    for (i = 0; i < 4; i++)
////        //write(buffer, STRIP_SCI + 4 * i, data[i]);
////        write(buffer, STRIP_SCI + 4 * (4 - i - 1), data[i]);
////
////    w = 0x80000000 | (len << 24) | (bitMask << 16) | (inst << 10) | (addr << 5) |
/// chipId;
////
////    ierr = write(buffer, STRIP_SC_CSR, w);
////
////    waitClear(buffer, STRIP_SC_CSR, 0x80000000);
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
////                                      unsigned int channel, const ROCStream&
/// rocStream)
////{
////    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
////    makeMaskSequence(sequence, channel, rocStream, "Kill");
////    //    makeMaskSequence(sequence, channel, rocStream, "Inject");
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
////                                      unsigned int channel, const ROCStream&
/// rocStream)
////{
////    std::cout << __COUT_HDR_FL__ << "Making mask! " << std::endl;
////    makeMaskSequence(sequence, channel, rocStream, "Kill");
////    //    makeMaskSequence(channel,rocStream,sequence,"Inject");
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint64_t>& sequence,
////                                      unsigned int channel, const ROCStream&
/// rocStream, /                                      const std::string& registerName)
////{
////    int chipId = rocStream.getFEWROCAddress();//9, 14 or 21 bcast
////    std::string mask = rocStream.getROCMask();
////    std::cout << __COUT_HDR_FL__ << "Mask length: " << mask.length() << std::endl;
////
////    uint64_t uInt64Data = 0;
////    std::string std::stringData = "";
////
////    //have to manually set every bit for mask writes.
////    //reset fifo
////    //download every bit (shift_in and shift_ctrl) to fifo (setup muxes and control
/// lines) /    //configure muxes for readout /    //commence readout
////
////    //FIFO Controls - byte 5 (7:0)
////    //7 - FIFO Clock
////    //6 - Shift Ctrl bit
////    //MASK_CELL_H - Shift In bit
////    //MUX Controls - byte 0 (7:0)
////    //7 - Read En/Output MUX sel (1 for masks)
////    //1 - Write En for mux
////    //0 - Reset Fifo
////
////    //reset fifo
////    BitManipulator::insertBits(uInt64Data, 0x1, 56, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////
////    //configure fifo for write
////    BitManipulator::insertBits(uInt64Data, 0x2, 56, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////
////    //Download every bit (shift_in and shift_control) to fifo (setup muxes and control
/// lines) /    //Bit 7 of data is FIFO clock, bit 6 is shift_control, bit 5 is shift_in /
/////start bits (ctrl 0, data 0 => ctrl 1, data 0)
////
////    BitManipulator::insertBits(uInt64Data, 0x40, 16, 8);//(0<<7) | (1<<6) | (0<<5)
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////    BitManipulator::insertBits(uInt64Data, 0xc0, 16, 8);//(1<<7) | (1<<6) | (0<<5) ->
/// clock the data in the fifo /    sequence.pushBack(ChannelFIFOAddress[channel],
/// uInt64Data);
////
////    std::stringData = PSI46DigROCDefinitions::makeMaskWriteCommand(chipId,
////                 registerName, mask);
////
////    uint8_t data;
////    for (unsigned int s = 0; s < std::stringData.length(); s++)
////        for (int b = 8 - 1; b >= 0 && (s * 8 + 8 - b < 13 + 128); b--)
////        {
////            //std::cout << __COUT_HDR_FL__ << "S: " << s << " val: " <<
/// std::stringData.data()[s] << " b: " << b << " bit: " << ((stringData.data()[s]>>b)&1)
///<< std::endl; /            data = 0x40 | (((stringData.data()[s] >> b) & 1) << 5); /
/// BitManipulator::insertBits(uInt64Data, (uint64_t) data, 16, 8); /
/// sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data); /            data |= 0x80;
////            BitManipulator::insertBits(uInt64Data, (uint64_t) data, 16, 8);
////            sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////        }
////
////    //reset Shift Control
////    BitManipulator::insertBits(uInt64Data, 0x0, 16, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////    BitManipulator::insertBits(uInt64Data, 0x80, 16, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////
////    //configure muxes for readout
////    BitManipulator::insertBits(uInt64Data, 0x0, 56, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////
////    //commence readout
////    BitManipulator::insertBits(uInt64Data, 0x80, 56, 8);
////    sequence.pushBack(ChannelFIFOAddress[channel], uInt64Data);
////
////}
////
//////==============================================================================
////void PSI46DigFirmware::makeMaskSequence(FirmwareSequence<uint32_t>& sequence,
////                                      unsigned int channel, const ROCStream&
/// rocStream, /                                      const std::string& registerName)
////{}
////
//////==============================================================================
////unsigned int PSI46DigFirmware::write(std::string& buffer, unsigned int address,
/// unsigned int data)
////{
////    unsigned int begin = buffer.length();
////    unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
////    buffer.resize(buffer.length() + 12, '\0');
////    if (begin != 0)
////        begin -= 1;
////    buffer[begin + 0] = numberOfBufferedCommands + 1;
////    buffer[begin + 1] = WRITE;
////    buffer[begin + 2] = 0;
////    buffer[begin + 3] = 8;
////    buffer[begin + 4] = (address >> 24) & 0xff;
////    buffer[begin + 5] = (address >> 16) & 0xff;
////    buffer[begin + 6] = (address >> 8) & 0xff;
////    buffer[begin + 7] = address & 0xff;
////    buffer[begin + 8] = (data >> 24) & 0xff;
////    buffer[begin + 9] = (data >> 16) & 0xff;
////    buffer[begin + 10] = (data >> 8) & 0xff;
////    buffer[begin + 11] = data & 0xff;
////    if (begin == 0)
////        buffer += '\0';
////    return (unsigned int) buffer[begin] + 1;
////}
////
//////==============================================================================
////unsigned int PSI46DigFirmware::waitSet(std::string& buffer, unsigned int address,
/// unsigned int data, unsigned int timeout)
////{
////    unsigned int begin = buffer.length();
////    unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
////    buffer.resize(buffer.length() + 16, '\0');
////    if (begin != 0)
////        begin -= 1;
////    buffer[begin + 0] = numberOfBufferedCommands + 1;
////    buffer[begin + 1] = WAITSET;
////    buffer[begin + 2] = 0;
////    buffer[begin + 3] = 12;
////    buffer[begin + 4] = (address >> 24) & 0xff;
////    buffer[begin + 5] = (address >> 16) & 0xff;
////    buffer[begin + 6] = (address >> 8) & 0xff;
////    buffer[begin + 7] = address & 0xff;
////    buffer[begin + 8] = (data >> 24) & 0xff;
////    buffer[begin + 9] = (data >> 16) & 0xff;
////    buffer[begin + 10] = (data >> 8) & 0xff;
////    buffer[begin + 11] = data & 0xff;
////    buffer[begin + 12] = (timeout >> 24) & 0xff;
////    buffer[begin + 13] = (timeout >> 16) & 0xff;
////    buffer[begin + 14] = (timeout >> 8) & 0xff;
////    buffer[begin + 15] = timeout & 0xff;
////    if (begin == 0)
////        buffer += '\0';
////    return (unsigned int) buffer[begin] + 1;
////}
////
//////==============================================================================
////unsigned int PSI46DigFirmware::waitClear(std::string& buffer, unsigned int address,
/// unsigned int data, unsigned int timeout)
////{
////    unsigned int begin = buffer.length();
////    unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
////    buffer.resize(buffer.length() + 16, '\0');
////    if (begin != 0)
////        begin -= 1;
////    buffer[begin + 0] = numberOfBufferedCommands + 1;
////    buffer[begin + 1] = WAITCLR;
////    buffer[begin + 2] = 0;
////    buffer[begin + 3] = 12;
////    buffer[begin + 4] = (address >> 24) & 0xff;
////    buffer[begin + 5] = (address >> 16) & 0xff;
////    buffer[begin + 6] = (address >> 8) & 0xff;
////    buffer[begin + 7] = address & 0xff;
////    buffer[begin + 8] = (data >> 24) & 0xff;
////    buffer[begin + 9] = (data >> 16) & 0xff;
////    buffer[begin + 10] = (data >> 8) & 0xff;
////    buffer[begin + 11] = data & 0xff;
////    buffer[begin + 12] = (timeout >> 24) & 0xff;
////    buffer[begin + 13] = (timeout >> 16) & 0xff;
////    buffer[begin + 14] = (timeout >> 8) & 0xff;
////    buffer[begin + 15] = timeout & 0xff;
////    if (begin == 0)
////        buffer += '\0';
////    return (unsigned int) buffer[begin] + 1;
////}
////
//////==============================================================================
////unsigned int PSI46DigFirmware::read(std::string& buffer, unsigned int address)
////{
////    unsigned int begin = buffer.length();
////    unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
////    buffer.resize(buffer.length() + 8, '\0');
////    if (begin != 0)
////        begin -= 1;
////    buffer[begin + 0] = numberOfBufferedCommands + 1;
////    buffer[begin + 1] = READ;
////    buffer[begin + 2] = 0;
////    buffer[begin + 3] = 4;
////    buffer[begin + 4] = (address >> 24) & 0xff;
////    buffer[begin + 5] = (address >> 16) & 0xff;
////    buffer[begin + 6] = (address >> 8) & 0xff;
////    buffer[begin + 7] = address & 0xff;
////    if (begin == 0)
////        buffer += '\0';
////    return (unsigned int) buffer[begin] + 1;
////}
////
//////==============================================================================
////string PSI46DigFirmware::readCSRRegister()
////{
////    std::string buffer;
////    read(buffer,STRIP_CSR);
////    return buffer;
////}
////
//////==============================================================================
////unsigned int PSI46DigFirmware::getNumberOfBufferedCommands(std::string& buffer)
////{
////    if (buffer.length() == 0)
////        return 0;
////    unsigned int bufferPosition = 0;
////    unsigned int commandNumber = 0;
////    while (bufferPosition < buffer.length() - 1)
////    {
////        bufferPosition += (unsigned int) buffer[bufferPosition + 3] + 4;
////        ++commandNumber;
////    }
////    return commandNumber;
////}
////
//////==============================================================================
////string PSI46DigFirmware::compareSendAndReceive(const std::string& sentBuffer,
/// std::string& acknowledgment)
////{
////    std::string reSendBuffer;
////    unsigned int skipBuffer = 0;
////    unsigned int skipAcknow = 0;
////    unsigned int remainBufferCommands;
////    unsigned int remainAcknowCommands;
////
////    std::cout << __COUT_HDR_FL__ << "Comparing sent buffer and received
/// acknowledgment!!!" << std::endl;
////
////    std::cout << __COUT_HDR_FL__ << "Buffer size (send): " << sentBuffer.size() <<
/// std::endl; /    std::cout << __COUT_HDR_FL__ << "Acknowledgment size (receive): " <<
/// acknowledgment.size() << std::endl;
////
////    while(skipBuffer < sentBuffer.size())
////    {
////
////        for (int position = 0; position < 4; position++)
////        {
////            if (position + skipBuffer >= sentBuffer.size()) //make sure the number
/// NEVER goes above the limit /            { /                reSendBuffer.clear(); /
/// std::cout << __COUT_HDR_FL__ << "Done... Works!!!" << std::endl; /
/// return reSendBuffer; /            }
////
////            if (position == 0)
////            {
////                if (sentBuffer[position + skipBuffer] != acknowledgment[position +
/// skipAcknow]) /                { /                    std::cout << __COUT_HDR_FL__ <<
///"ERROR - Sent " << position + skipBuffer << "th: " << hex << (unsigned
/// int)sentBuffer[position + skipBuffer] << dec /                    << " different from
/// Received " << position + skipAcknow << "th:" /                    << hex << (unsigned
/// int)acknowledgment[position + skipAcknow] << dec << std::endl; /
/// std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl; /
/// reSendBuffer.clear(); /                    for (unsigned int i = skipBuffer; i <
///(skipBuffer + 4 + sentBuffer[skipBuffer + 3]); i++) /                    { /
/// reSendBuffer+= sentBuffer[i]; /                    } /                    return
/// reSendBuffer; /                } /            }
////
////            if (position == 1)
////            {
////                if ( (sentBuffer[position + skipBuffer]) != (acknowledgment[position +
/// skipAcknow] & 0xf) ) /                { /                    std::cout <<
///__COUT_HDR_FL__ << "ERROR - Sent " << position + skipBuffer << "th: " << hex <<
///(unsigned int)sentBuffer[position + skipBuffer] << dec /                    << "
/// different from Received " << position + skipAcknow << "th:" /                    <<
/// hex
///<< (unsigned int)acknowledgment[position + skipAcknow] << dec << std::endl; /
/// std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl; /
/// reSendBuffer.clear(); /                    for (unsigned int i = skipBuffer; i <
///(skipBuffer + 4 + sentBuffer[skipBuffer + 3]); i++) /                    { /
/// reSendBuffer+= sentBuffer[i]; /                    } /                    return
/// reSendBuffer; /                } /            }
////
////            if (position == 2)
////            {
////                if ( (sentBuffer[position + skipBuffer] != 0) ||
///(acknowledgment[position + skipAcknow] != 0) ) /                { /
/// std::cout << __COUT_HDR_FL__ << "ERROR - Sent " << position + skipBuffer << "th: " <<
/// hex << (unsigned int)sentBuffer[position + skipBuffer] << dec /                    <<
/// "  different from Received " << position + skipAcknow << "th:" /                    <<
/// hex
///<< (unsigned int)acknowledgment[position + skipAcknow] << dec << std::endl; /
/// std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl; /
/// reSendBuffer.clear(); /                    for (unsigned int i = skipBuffer; i <
///(skipBuffer + 4 + sentBuffer[skipBuffer + 3]); i++) /                    { /
/// reSendBuffer+= sentBuffer[i]; /                    } /                    return
/// reSendBuffer; /                } /            }
////
////            if ( position == 3)
////            {
////                remainBufferCommands = sentBuffer[position + skipBuffer]; //4th bit
/// tells us how many bits are left /                remainAcknowCommands =
/// acknowledgment[position + skipAcknow];
////
////                if ( ((remainBufferCommands == 12) && (remainAcknowCommands == 4)) ||
///((remainBufferCommands == 8) && (remainAcknowCommands == 0)) ) /                { /
/// skipBuffer += (4 + remainBufferCommands); //go to the beginning of next buffer /
/// skipAcknow += (4 + remainAcknowCommands); /                } /                else /
///{ /                    std::cout << __COUT_HDR_FL__ << "ERROR - Sent " << position +
/// skipBuffer << "th: " << hex << (unsigned int)sentBuffer[position + skipBuffer] << dec
////                    << " not compatible with Received " << position + skipAcknow <<
///"th:" /                    << hex << (unsigned int)acknowledgment[position +
/// skipAcknow] << dec << std::endl; /                    std::cout << __COUT_HDR_FL__ <<
///"Position: " << position << std::endl; /                    reSendBuffer.clear(); /
/// for (unsigned int i = skipBuffer; i < (skipBuffer + 4 + sentBuffer[skipBuffer + 3]);
/// i++) /                    { /                        reSendBuffer+= sentBuffer[i]; /
///} /                    return reSendBuffer; /                } /            } /
///} /    }
////
////    reSendBuffer.clear();
////    std::cout << __COUT_HDR_FL__ << "Done... Works!!!" << std::endl;
////    return reSendBuffer;
////}
////
//////==============================================================================
////uint32_t PSI46DigFirmware::createRegisterFromValue(std::string& readBuffer,
/// std::string& receivedValue)
////{
////    for (int position = 0; position < 8; position++)
////    {
////        if (position == 0 || position == 4 || position == 5 || position == 6 ||
/// position == 7) /        { /            if (readBuffer[position] !=
/// receivedValue[position]) /            { /                std::cout << __COUT_HDR_FL__
///<< "ERROR - Read " << position << "th: " << hex << (unsigned int)readBuffer[position]
///<< dec /                << " different from Received " << position << "th:" /
///<< hex << (unsigned int)receivedValue[position] << dec << std::endl; /
/// std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl; /
/// return 0; /            } /        }
////
////        if (position == 1)
////        {
////            if ( (readBuffer[position] != (receivedValue[position] & 0xf)) )
////            {
////                std::cout << __COUT_HDR_FL__ << "ERROR - Read " << position << "th: "
///<< hex << (unsigned int)readBuffer[position] << dec /                << " different
/// from Received " << position << "th:" /                << hex << (unsigned
/// int)receivedValue[position] << dec << std::endl; /                std::cout <<
///__COUT_HDR_FL__ << "Position: " << position << std::endl; /                return 0; /
///} /        }
////
////        if (position == 2)
////        {
////            if ( (readBuffer[position] != 0) || (receivedValue[position] != 0) )
////            {
////                std::cout << __COUT_HDR_FL__ << "ERROR - Sent " << position << "th: "
///<< hex << (unsigned int)readBuffer[position] << dec /                << " different
/// from Received " << position << "th:" /                << hex << (unsigned
/// int)receivedValue[position] << dec << std::endl; /                std::cout <<
///__COUT_HDR_FL__ << "Position: " << position << std::endl; /                return 0; /
///} /        }
////
////        if ( position == 3)
////        {
////            if ( (readBuffer[position] != 4) || (receivedValue[position] != 8)  )
////            {
////                std::cout << __COUT_HDR_FL__ << "ERROR - Read " << position << "th: "
///<< hex << (unsigned int)readBuffer[position] << dec /                << " not
/// compatible with Received " << position << "th:" /                << hex << (unsigned
/// int)receivedValue[position] << dec << std::endl; /                std::cout <<
///__COUT_HDR_FL__ << "Position: " << position << std::endl; /                return 0; /
///} /        } /    }
////
////    uint32_t registerValue;
////
////    registerValue = ((unsigned int)receivedValue[8] << 24)
////                    + ((unsigned int)receivedValue[9] << 16)
////                    + ((unsigned int)receivedValue[10] << 8)
////                    + (unsigned int)receivedValue[11];
////
////    std::cout << __COUT_HDR_FL__ << "No problem encountered! Register value created!!!
///" << std::endl;
////
////    return registerValue;
////}
////
//////==============================================================================
////void PSI46DigFirmware::setFrequencyFromClockState(std::string& buffer, double
/// frequency)
////{
////    std::cout << __COUT_HDR_FL__ << "Setting up clock frequency!!!" << std::endl;
////
////    int quotient;
////    int numerator;
////    int denominator;
////    double realClockFrequency;
////
////    if (BitManipulator::readBits(stripCSRRegisterValue_, 17, 1) == 1) //if fastBCO is
/// enabled /        quotient = 4; /    else //normal cases /        quotient = 8;
////
////    if (isClockStateExternal()) //base freq: 54MHz
////    {
////        realClockFrequency = EXTERNAL_CLOCK_FREQUENCY / quotient; //this is the REAL
/// frequency being used /    } /    else //base freq: 66.667MHz /    { /
/// realClockFrequency = INTERNAL_CLOCK_FREQUENCY / quotient; //this is the REAL frequency
/// being used /    }
////
////    double factor = frequency / realClockFrequency;
////
////    //The device needs the denominator and the denominator to be load into a 5 bit
/// register /    //It will take two initial numerator and denominator bigger than
/// necessary (to do not loose precision) /    //and divide them for their gcd. If they
/// still do not fit in 5 bit, they are trunked (divided by 2) /    //untill they are less
/// than 32 /    numerator = factor * 100; //we will work with 2 digits precision after
/// the  decimal point /    denominator = 100;
////
////    do
////    {
////        //We will need the GCD at some point in order to simplify fractions //taken
/// from other sources /        int gcd = numerator; /        int rest = denominator; /
/// int tmp;
////
////        while (rest != 0)
////        {
////            tmp = rest;
////            rest = gcd % tmp;
////            gcd = tmp;
////        }
////        //Done finding the GCD
////
////        if (gcd == 1) //if there's no GCD, just divide by 2 to find the nearest
/// approssimation with less bits /        { /            numerator /= 2; /
/// denominator /= 2; /        } /        else /        { /            numerator /= gcd; /
/// denominator /= gcd; /        }
////
////    }
////    while (denominator >= 32 || numerator >= 32);
////    std::cout << __COUT_HDR_FL__ << "Numerator: " << numerator << std::endl;
////    std::cout << __COUT_HDR_FL__ << "Denominator: " << denominator << std::endl;
////    setFrequencyRatio(buffer, numerator, denominator);
////    std::cout << __COUT_HDR_FL__ << "Done with clock frequency setup!!!" << std::endl;
////}
//////==============================================================================
////bool PSI46DigFirmware::isClockStateExternal() //returns true if the clock state is
/// External
////{
////    if (BitManipulator::readBits(stripCSRRegisterValue_, 16, 1) == 1)
////        return true;
////    else
////        return false;
////}
////
//////==============================================================================
////void PSI46DigFirmware::setCSRRegister(uint32_t total)
////{
////    stripCSRRegisterValue_ = total;
////}
////
//////==============================================================================
////void PSI46DigFirmware::setPacketSizeStripCSR(uint32_t size)
////{
////    if (size > 31)
////        std::cout << __COUT_HDR_FL__
////        << "ERROR: Maximum packet size is 31 while here you are trying to set "
////        << size << " packets!" << std::endl;
////    BitManipulator::insertBits(stripCSRRegisterValue_, size, 3, 5);
////    //write(buffer,STRIP_CSR, stripSCRRegisterValue_);
////}
////
//////==============================================================================
////void PSI46DigFirmware::enableChannelsStripCSR(bool channel0, bool channel1,
////        bool channel2, bool channel3, bool channel4, bool channel5)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, ((uint32_t) channel0)
////                                 + ((uint32_t) channel1 << 1) + ((uint32_t) channel2
///<< 2) /                                 + ((uint32_t) channel3 << 3) + ((uint32_t)
/// channel4 << 4) /                                 + ((uint32_t) channel5 << 5), 8, 6);
////}
////
//////==============================================================================
////void PSI46DigFirmware::setExternalBCOClockSourceStripCSR(std::string clockSource)
////{
////    if (clockSource == "External")
////        BitManipulator::insertBits(stripCSRRegisterValue_, 1, 16, 1);
////    else if (clockSource == "Internal")
////        BitManipulator::insertBits(stripCSRRegisterValue_, 0, 16, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::setHaltStripCSR(bool set)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) set, 17, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::enableBCOStripCSR(bool enable)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 19, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::flushBuffersStripCSR(void)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 20, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetTriggerCounterStripCSR(string& buffer)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 21, 1);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////
////    BitManipulator::insertBits(stripCSRRegisterValue_, 0, 21, 1);
////    write(buffer, STRIP_CSR, stripCSRRegisterValue_);
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetBCOCounterStripCSR(void)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, 1, 22, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::enableTriggerStripCSR(bool enable)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 23, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::sendTriggerDataStripCSR(bool send)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 24, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::sendTriggerNumberStripCSR(bool send)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 25, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::sendBCOStripCSR(bool send)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) send, 26, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::enableStreamStripCSR(bool enable)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) enable, 27, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetDCMStripCSR(bool clear)
////{
////    BitManipulator::insertBits(stripCSRRegisterValue_, (uint32_t) clear, 31, 1);
////}
////
//////==============================================================================
////uint32_t PSI46DigFirmware::waitDCMResetStripCSR(void)
////{
////    uint32_t bitToCheck = 0;
////    BitManipulator::insertBits(bitToCheck, 1, 31, 2);
////    return bitToCheck;
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetDAC(void)
////{
////    BitManipulator::insertBits(stripResetRegisterValue_, 1, 27, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetLink(bool channel0, bool channel1, bool channel2,
////                               bool channel3, bool channel4, bool channel5)
////{
////    stripResetRegisterValue_ = 0;
////    BitManipulator::insertBits(stripResetRegisterValue_,
////                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
////                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3
///<< 3) /                                 + ((uint32_t) channel4 << 4) + ((uint32_t)
/// channel5 << 5), /                                 0, 6); /
/// BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1); /    /* /
/// write(buffer, STRIP_RESET, (1<<28) + /     ((uint32_t)channel0) + /
///((uint32_t)channel1<<1) + /     ((uint32_t)channel2<<2) + /     ((uint32_t)channel3<<3)
///+ /     ((uint32_t)channel4<<4) + /     ((uint32_t)channel5<<5) /     ); /     */
////}
////
//////==============================================================================
////void PSI46DigFirmware::clearErrors(bool channel0, bool channel1, bool channel2,
////                                 bool channel3, bool channel4, bool channel5)
////{
////    stripResetRegisterValue_ = 0;
////    BitManipulator::insertBits(stripResetRegisterValue_,
////                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
////                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3
///<< 3) /                                 + ((uint32_t) channel4 << 4) + ((uint32_t)
/// channel5 << 5), /                                 0, 6); /
/// BitManipulator::insertBits(stripResetRegisterValue_, 1, 29, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::clearFIFO(bool channel0, bool channel1, bool channel2,
////                               bool channel3, bool channel4, bool channel5)
////{
////    stripResetRegisterValue_ = 0;
////    BitManipulator::insertBits(stripResetRegisterValue_,
////                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
////                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3
///<< 3) /                                 + ((uint32_t) channel4 << 4) + ((uint32_t)
/// channel5 << 5), /                                 0, 6); /
/// BitManipulator::insertBits(stripResetRegisterValue_, 1, 30, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::resetChip(bool channel0, bool channel1, bool channel2,
////                               bool channel3, bool channel4, bool channel5)
////{
////    stripResetRegisterValue_ = 0;
////    BitManipulator::insertBits(stripResetRegisterValue_,
////                                 ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
////                                 + ((uint32_t) channel2 << 2) + ((uint32_t) channel3
///<< 3) /                                 + ((uint32_t) channel4 << 4) + ((uint32_t)
/// channel5 << 5), /                                 0, 6); /
/// BitManipulator::insertBits(stripResetRegisterValue_, 1, 31, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::setFrequencyRatio(string& buffer, int numerator, int
/// denominator)
////{
////    //The device need to load numerator minus one and denominator minus one, with an
/// internal address of 0x50 and 052 respectively /    write(buffer, STRIP_BCO_DCM,
/// 0x80500000 + (numerator - 1)); //  Set BCOCLK numerator // was 0x80500003 /
/// waitClear(buffer, STRIP_BCO_DCM, 0xf0000000); //  Wait DCM write to finish // was
/// 0x80000000
////
////    write(buffer, STRIP_BCO_DCM, 0x80520000 + (denominator - 1)); //  Set BCOCLK
/// denominator // was 0x80520001 /    waitClear(buffer, STRIP_BCO_DCM, 0xf0000000); //
/// Wait DCM write to finish - BCO frequency should be 13.513 MHz // was 0x80000000
////}
////
//////==============================================================================
////void PSI46DigFirmware::BCOOffset(uint32_t offset)
////{
////    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, offset, 0, 4);
////}
////
//////==============================================================================
////void PSI46DigFirmware::selectSpyFIFO(uint32_t input)
////{
////    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, input, 4, 3);
////}
////
//////==============================================================================
////void PSI46DigFirmware::halt(bool halt)
////{
////    BitManipulator::insertBits(stripTriggerCSRRegisterValue_, (uint32_t) halt, 7, 1);
////}
////
//////==============================================================================
////void PSI46DigFirmware::configureStripTriggerUnbiased(string& buffer)
////{
////    write(buffer, STRIP_TRIG_UNBIASED, 0x002805c); //  Configure unbiased trigger
////}
////
//////==============================================================================
////void PSI46DigFirmware::configureTriggerInputs(string& buffer)
////{
////    write(buffer, STRIP_TRIG_INPUT_0, 0x3f440000); //  PSI46Dig GOTHIT trigger input
/// channel 0,1 /    write(buffer, STRIP_TRIG_INPUT_1, 0x3f440000); //  PSI46Dig GOTHIT
/// trigger input channel 2,3 /    write(buffer, STRIP_TRIG_INPUT_2, 0x0);        //
/// PSI46Dig GOTHIT trigger input channel 4,5
////}
////
//
