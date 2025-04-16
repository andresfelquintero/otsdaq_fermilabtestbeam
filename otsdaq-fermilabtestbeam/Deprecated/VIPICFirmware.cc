//#include "otsdaq-components/DAQHardware/VIPICFirmware.h"
//#include "otsdaq-components/DAQHardware/VIPICFirmwareDefinitions.h"
//#include "otsdaq/BitManipulator/BitManipulator.h"
//#include "otsdaq-components/DetectorHardware/VIPICROCDefinitions.h"
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
//// Updated from Jones' mails at 10 September 2014factor
//
//// VIPIC_CSR
//
//// Bit 31 . . . . . Resets the DCM components (not self clearing)
//// Bit 30 . . . . . DCM lock status (read only)
//// Bit 29 . . . . . unused
//// Bit 28 . . . . . Resets the network stream interface (not self clearing)
//// Bit 27..25 . . . unused
//// Bit 24 . . . . . Enables sending trigger data on network stream
//// Bit 23 . . . . . Enables the network stream interface
//// Bit 22 . . . . . TS counter reset
//// Bit 21 . . . . . Clears the trigger counter
//// Bit 20 . . . . . Enables sending of TS counter on network stream
//// Bit 19 . . . . . Arm TS reset logic
//// Bit 18 . . . . . unused
//// Bit 17 . . . . . Halts all readout processes when set
//// Bit 16 . . . . . Selects external reference clock
//// Bit 15..13 . . . Selects debug information to present on vipic debug output
//// Bit 9..8 . . . . Specifies delay before flushing partially filled packets
//// Bit 7..3 . . . . Specifies packet size
//// Bit 2..0 . . . . not used
//
//
//// VIPIC_CLOCK
//
//// Bit  7..0 . . . readout period in TS_clk cycles.  The next readout
////                  sequence starts after this amount of time unless a
////                  readout sequence is still in progress.  In that case
////                  the next sequence starts on the next TS_clk edge.
//// Bit 15..8 . . . Number of reads in a readout cycle.
//// Bit 19..16  . . Number of Serial_clk cycles (minus one) between readout
////                  cycles.  This gives enough time for rstrobe to propagate
////                  through the logic on the chip.  I usually use 3, which
////                  pauses for 4 serial_clk cycles.
//// Bit 23..20  . . These specify which signals to assert on every read cycle:
////                  Bit 20 - assert clean at the end of the read cycle
////                  Bit 21 - send TS_clk signals
//// Bit 22  . . . .  unused
//// Bit 23  . . . .  unused
//// Bit 24  . . . . Write 1 to invoke the state machine that shifts the S
////                  bits.  Read back as 1 when the state machine is busy
////                  shifting.
//// Bit 25  . . . . Same as bit 24 but controls the R bits.
//// Bit 26  . . . . Same as bit 24 but controls the D bits.
//// Bit 27  . . . . Asserts the load signal for one serial_clk cycle.
//// Bit 29  . . . . Asserts the restart signal for one serial_clk cycle.
//// Bit 30  . . . . Send one ts_clk pulse
//// Bit 31  . . . . Sends one ready cycle
//
//
//
////==============================================================================
// VIPICFirmware::VIPICFirmware(unsigned int version, std::string type) :
//		FrontEndFirmwareBase(version),
//        pixelCSRRegisterValue_(0)
//{
//	protocolInstance_ = FrontEndFirmwareBase::getInstance(type,version);
//	assert(protocolInstance_ != NULL);
//	//TODO if asserts are disabled/ignored still should check that protocol instance isn't
// null
//	//returns either new OtsUDPFirmware or new PurdueFirmwareCore,
//	//now we can call write/read etc with protocol->write, protocol->read, etc
//}
//
////==============================================================================
// VIPICFirmware::~VIPICFirmware()
//{
//	delete protocolInstance_;
//	protocolInstance_= NULL;
//}
//
////==============================================================================
// int VIPICFirmware::init()
//{
//    return 0;
//}
//
////==============================================================================
// std::string VIPICFirmware::configureClocks(std::string source, double frequency)
//{
//    std::cout << __COUT_HDR_FL__ << "Writing Clock configuration!" << std::endl;
//
//    std::string buffer;
//
//    setTimeStampClockSource(source);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_); //  Reset CSR -
//    reset trigger counter, external 27 MHz clock
//
//    resetDCMCSR(true);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_); //  Set reset
//    to DCM
//
//    resetDCMCSR(false);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_); //  Clear reset
//    to DCM
//
//    setFrequencyFromClockState(buffer, frequency);
//
//    protocolInstance_->waitSet(buffer, VIPIC_CSR,  1 << 30); //  Wait for DCM to lock
//
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue :" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string VIPICFirmware::resetBCO(void)
//{
//    std::string buffer;
//
//    resetDCMCSR(true);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_); //  Set reset
//    to DCM
//
//    resetDCMCSR(false);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_); //  Clear reset
//    to DCM
//
//    protocolInstance_->waitSet(buffer, VIPIC_CSR, 1 << 30); //  Wait for DCM to lock
//
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue :" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl;
//
//    return buffer;
//}
////==============================================================================
// std::string VIPICFirmware::resetDetector(int reset)
//{
//    std::cout << __COUT_HDR_FL__ << "Resetting detector!" << std::endl;
//    std::string buffer;
//    //Documentation 5.2.1
//    //FIXME Sets DCM_RESET, TRIG_ENABLE
//    //    write(buffer, Pixel_RESET, 0x80200000);
//    //    waitSet(buffer, Pixel_RESET, 0x80000000);
//    //    //From Jones' example it appears DMC_RESET does not clear by it self and must
//    be manually cleared
//    //    write(buffer, Pixel_RESET, 0x00200000);
//    //    waitClear(buffer, Pixel_RESET, 0x80000000);
//    return buffer;
//}
//
////==============================================================================
// std::string VIPICFirmware::enableTrigger(bool enable)
//{
//    std::string buffer;
//    std::cout << __COUT_HDR_FL__ << "Enabling Trigger!!!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue in :" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl;
//
//    enableTriggerDataStreamPixelCSR(enable);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_);
//
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue out:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done enabling Trigger!!!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// void VIPICFirmware::setTimeStampClockSource(std::string clockSource)
//{
//    if (clockSource == "External")
//        setTimeStampClockSourcePixelCSR(true);
//    else if (clockSource == "Internal")
//        setTimeStampClockSourcePixelCSR(false);
//    else
//    {
//        std::cout << __COUT_HDR_FL__ << "Configuration Error:\t" << std::endl;
//        std::cout << __COUT_HDR_FL__ << clockSource << " is not a valid clock source
//        (Internal/External values allowed only)"<< std::endl; assert(0);
//    }
//}
//
////==============================================================================
// std::string VIPICFirmware::resetTimeStamp()
//{
//    std::cout << __COUT_HDR_FL__ << "Reset Time Stamp!!!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue in :" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::string buffer;
//
//    ////    resetTriggerCounterPixelCSR(buffer);
//    //    write(buffer, PIXEL_CSR, pixelCSRRegisterValue_);
//    //    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue :" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl;
//    //
//    //    // enableTimeStampPixelCSR(true);
//    //    write(buffer, PIXEL_CSR, pixelCSRRegisterValue_);
//    //    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue out:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl;
//    //    std::cout << __COUT_HDR_FL__ << "Done reset Time Stamp!!!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string VIPICFirmware::startStream()
//{
//    std::cout << __COUT_HDR_FL__ << "Start Stream!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue in:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::string buffer;
//
//    enableStreamPixelCSR(true);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_);
//
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue out:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done start Stream!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string VIPICFirmware::setUp()
//{
//    std::cout << __COUT_HDR_FL__ << "Start Stream!" << std::endl;
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue in:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::string buffer;
//
//    setPacketSizePixelCSR(6);
//    prepareResetTimeStampCSR();
//    clearTriggerCounterPixelCSR();
//    enableStreamPixelCSR(true);
//    enableTriggerDataStreamPixelCSR(true);
//
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_);
//
//    std::cout << __COUT_HDR_FL__ << "pixelCSRRegisterValue out:" << std::hex <<
//    pixelCSRRegisterValue_ << std::dec << std::endl; std::cout << __COUT_HDR_FL__ <<
//    "Done start Stream!" << std::endl;
//
//    return buffer;
//}
//
////==============================================================================
// std::string VIPICFirmware::stopStream()
//{
//    std::string buffer;
//    enableTriggerDataStreamPixelCSR(false);
//    protocolInstance_->write(buffer, VIPIC_CSR, pixelCSRRegisterValue_);
//    return buffer;
//}
//
//
////==============================================================================
// void VIPICFirmware::makeMaskBuffer(std::string& buffer, unsigned int channel, const
// ROCStream& rocStream)
//{
//    std::cout << __COUT_HDR_FL__ << "\tMaking mask! " << std::endl;
//    std::string mask = rocStream.getROCMask();
//    std::cout << __COUT_HDR_FL__ << "\tMask length: " << mask.length() << std::endl;
//
//    //FIXME: missing configure data (12 bits)
//
//    //  0   =>  set = 0 reset = 0 Normal Mode
//    //  1   =>  set = 0 reset = 1 Killed Mode
//    //  2   =>  set = 2 reset = 0 Deparsified Mode (the pixel should send data even when
//    there is not hit
//    //  3   =>  set = 1 reset = 1 Both (it should be like the Deparsified Mode above)
//    const int length = mask.size();
//    for (int i = 0; i < length; ++i)
//    {
//        switch(mask.at(i))
//        {
//        case '0':
//            maskSetPixel(i, 0);
//            maskOffPixel(i, 0);
//            break;
//        case '1':
//            maskSetPixel(i, 0);
//            maskOffPixel(i, 1);
//            break;
//        case '2':
//            maskSetPixel(i, 1);
//            maskOffPixel(i, 0);
//            break;
//        case '3':
//            maskSetPixel(i, 1);
//            maskOffPixel(i, 1);
//            break;
//        default:
//            std::cout << __COUT_HDR_FL__ << "Configuration Error: " << "in pixel " << i
//            << "(x = " << pixelColumn(i) << " y = " << pixelRow(i) << ") "
//            <<  mask.at(i) << " is not a valid mask configuration value (only 0 1 2 3
//            are allowed)"; assert(0); break;
//        }
//    }
//
//    for (int i = 0; i < PIXELS_SIDE * PIXELS_SIDE / 32; ++i)
//    {
//        protocolInstance_->write(buffer, VIPIC_SET_WRITE_BASE, setBuffer_[i]);
//        protocolInstance_->write(buffer, VIPIC_RESET_WRITE_BASE, resetBuffer_[i]);
//    }
//
//    shiftBufferSetCLK(1);
//    shiftBufferResetCLK(1);
//
//    for (int i = 0; i < PIXELS_SIDE * PIXELS_SIDE / 2; ++i)
//    {
//        protocolInstance_->write(buffer, VIPIC_DATA_WRITE_BASE, 0xFFF);
//    }
//    shiftBufferConfigurationCLK(1);
//
//
//    protocolInstance_->write(buffer, VIPIC_CLOCK, pixelClockRegisterValue_);
//    protocolInstance_->waitClear(buffer, VIPIC_CLOCK, (1 << 24) | (1 << 25) | (1 <<
//    26));
//}
//
////==============================================================================
// void VIPICFirmware::setFrequencyFromClockState(std::string& buffer, double frequency)
//{
//    std::cout << __COUT_HDR_FL__ << "Setting up clock frequency!!!" << std::endl;
//
//    int numerator;
//    int denominator;
//    double realClockFrequency;
//
//    int const quotient = 4;
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
//        //The GCD has been found
//
//        if (gcd == 1) //if there's no GCD bigger than one, just divide by 2 to find the
//        nearest approssimation with less bits
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
//
//    std::cout << __COUT_HDR_FL__ << "Numerator: " << numerator << std::endl;
//    std::cout << __COUT_HDR_FL__ << "Denominator: " << denominator << std::endl;
//    setFrequencyRatio(buffer, numerator, denominator);
//    std::cout << __COUT_HDR_FL__ << "Done with clock frequency setup!!!" << std::endl;
//}
//
////==============================================================================
// bool VIPICFirmware::isClockStateExternal()
//{
//    return isClockStateExternalCSR();
//}
//
////==============================================================================
// bool VIPICFirmware::isClockLocked()
//{
//    return isClockLockedCSR();
//}
//
//
////==============================================================================
// void VIPICFirmware::setFrequencyRatio(std::string& buffer, int numerator, int
// denominator)
//{
//    //The device need to load numerator minus one and denominator minus one, with an
//    internal address of 0x50 and 0x52 respectively protocolInstance_->write(buffer,
//    VIPIC_DCM_DATA, 0x80500000 + (numerator - 1)); //  Set Time StampCLK numerator
//    protocolInstance_->waitClear(buffer, VIPIC_DCM_DATA, 0xf0000000); //  Wait DCM write
//    to finish
//
//    protocolInstance_->write(buffer, VIPIC_DCM_DATA, 0x80520000 + (denominator - 1)); //
//    Set Time StampCLK denominator protocolInstance_->waitClear(buffer, VIPIC_DCM_DATA,
//    0xf0000000); //  Wait DCM write to finish - Time Stamp frequency should be 13.513
//    MHz
//}
//
///*
// * Local Registers Reset functions
// */
//
////==============================================================================
// void VIPICFirmware::resetPixelCSRRegisterValue()
//{
//    pixelCSRRegisterValue_ = 0;
//}
//
////==============================================================================
// void VIPICFirmware::resetPixelDCMRegisterValue()
//{
//    pixelDCMRegisterValue_ = 0;
//}
//
////==============================================================================
// void VIPICFirmware::resetPixelClockRegisterValue()
//{
//    pixelClockRegisterValue_ = 0;
//}
//
////==============================================================================
// void VIPICFirmware::resetPixelTestInjectRegisterValue()
//{
//    pixelTestInjectRegisterValue_ = 0;
//}
//
///*
// * Register layer manipulation functions
// */
//
////CSR
//
////==============================================================================
// void VIPICFirmware::setPacketSizePixelCSR(uint32_t size)
//{
//    //Bit 7..3 . . . . Specifies packet size
//    if (size > 31)
//    {
//        std::cout << __COUT_HDR_FL__ << "ERROR: Maximum packet size is 31 while here you
//        are trying to set " << std::endl; std::cout << __COUT_HDR_FL__ << size << "
//        packets!" << std::endl; assert(0);
//    }
//    BitManipulator::insertBits(pixelCSRRegisterValue_, size, 3, 5);
//}
//
////==============================================================================
// void VIPICFirmware::issueClearFIFOCSR(int delay)
//{
//    //FIXME: Waiting for specfication from Matthew
//    //Bit 9..8 . . . . Specifies delay before flushing partially filled packets
//
//
//    // BitManipulator::insertBits(pixelResetRegisterValue_,
//    //                              ((uint32_t) channel0) + ((uint32_t) channel1 << 1)
//    //                              + ((uint32_t) channel2 << 2) + ((uint32_t) channel3
//    << 3)
//    //                              + ((uint32_t) channel4 << 4) + ((uint32_t) channel5
//    << 5),
//    //                              0, 6);
//    // BitManipulator::insertBits(pixelResetRegisterValue_, 1, 30, 1);
//}
//
////==============================================================================
// bool VIPICFirmware::isClockStateExternalCSR() //returns true if the clock state is
// External
//{
//    //Bit 16 . . . . . Selects external reference clock
//    return BitManipulator::readBits(pixelCSRRegisterValue_, 16, 1);
//}
//
////==============================================================================
// void VIPICFirmware::setTimeStampClockSourcePixelCSR(bool externalClockSource)
//{
//    //Bit 16 . . . . . Selects external reference clock
//    BitManipulator::insertBits(pixelCSRRegisterValue_, externalClockSource, 16, 1);
//}
//
////==============================================================================
// void VIPICFirmware::prepareResetTimeStampCSR()
//{
//    //Prepares the logic to the global timestamp reset issued from the Master
//    //Bit 19 . . . . . Arm TS reset logic
//    BitManipulator::insertBits(pixelCSRRegisterValue_, 1, 19, 1);
//}
//
////==============================================================================
// void VIPICFirmware::enableTimeStampCounterPixelCSR(bool enable)
//{
//    //Bit 20 . . . . . Enables sending of TS counter on network stream
//    BitManipulator::insertBits(pixelCSRRegisterValue_, (uint32_t) enable, 20, 1);
//}
//
////==============================================================================
// void VIPICFirmware::clearTriggerCounterPixelCSR()
//{
//    //Bit 21 . . . . . Clears the trigger counter
//    BitManipulator::insertBits(pixelCSRRegisterValue_, 1, 21, 1);
//}
////==============================================================================
// void VIPICFirmware::immediateResetTimeStampCSR()
//{
//    //Resets the timestamp counter locally
//    //Bit 22 . . . . . TS counter reset
//    BitManipulator::insertBits(pixelCSRRegisterValue_, 1, 22, 1);
//}
//
////==============================================================================
// void VIPICFirmware::enableStreamPixelCSR(bool enable)
//{
//    //Bit 23 . . . . . Enables the network stream interface
//    BitManipulator::insertBits(pixelCSRRegisterValue_, (uint32_t) enable, 23, 1);
//}
//
////==============================================================================
// void VIPICFirmware::enableTriggerDataStreamPixelCSR(bool enable)
//{
//    //Bit 24 . . . . . Enables sending trigger data on network stream
//    BitManipulator::insertBits(pixelCSRRegisterValue_, (uint32_t) enable, 24, 1);
//}
//
////==============================================================================
// void VIPICFirmware::resetNetworkPixelCSR(bool reset)
//{
//    //Bit 28 . . . . . Resets the network stream interface (not self clearing)
//    BitManipulator::insertBits(pixelCSRRegisterValue_, (uint32_t) reset, 28, 1);
//}
//
////==============================================================================
// bool VIPICFirmware::isClockLockedCSR() //returns true if the clock is correctly locked
//{
//    //Bit 30 . . . . . DCM lock status (read only)
//    return BitManipulator::readBits(pixelCSRRegisterValue_, 30, 1);
//}
//
////==============================================================================
// void VIPICFirmware::resetDCMCSR(bool reset)
//{
//    //It should be called with 1, then a write must issued and then it must be called
//    with 0
//    //Bit 31 . . . . . Resets the DCM components (not self clearing)
//    BitManipulator::insertBits(pixelCSRRegisterValue_, reset, 31, 1);
//}
//
////"Clock" Register
//
////==============================================================================
// void VIPICFirmware::waitTimeStampToReadCLK(uint8_t delay)
//{
//    //Bit  7..0 . . . readout period in TS_clk cycles (delay)
//    BitManipulator::insertBits(pixelClockRegisterValue_, delay, 0, 8);
//}
//
////==============================================================================
// void VIPICFirmware::setReadsPerCycleCLK(uint8_t readsPerCycle)
//{
//    //Bit 15..8 . . . Number of reads in a readout cycle.
//    BitManipulator::insertBits(pixelClockRegisterValue_, readsPerCycle, 8, 8);
//}
//
////==============================================================================
// void VIPICFirmware::setWaitingPerCycleCLK(uint8_t delay)
//{
//    //Bit 19..16  . . Number of Serial_clk cycles (minus one) between readout
//    //Suggested value is 3
//    BitManipulator::insertBits(pixelClockRegisterValue_, delay, 16, 4);
//}
//
////==============================================================================
// void VIPICFirmware::assertCleanEndCLK(bool clean)
//{
//    //Bit 20 - assert clean at the end of the read cycle
//    BitManipulator::insertBits(pixelClockRegisterValue_, clean, 20, 1);
//}
//
////==============================================================================
// void VIPICFirmware::assertTimeStampEndCLK(bool timestamp)
//{
//    //Bit 21 - send TS_clk signals
//    BitManipulator::insertBits(pixelClockRegisterValue_, timestamp, 21, 1);
//}
//
////==============================================================================
// void VIPICFirmware::shiftBufferSetCLK(bool shift)
//{
//    //Bit 24  . . . . Write 1 to invoke the state machine that shifts the S
//    //It loads the S buffer to the chip
//    BitManipulator::insertBits(pixelClockRegisterValue_, shift, 24, 1);
//}
//
//
////==============================================================================
// void VIPICFirmware::shiftBufferResetCLK(bool shift)
//{
//    //Bit 25  . . . . Write 1 to invoke the state machine that shifts the R
//    //It loads the R buffer to the chip
//    BitManipulator::insertBits(pixelClockRegisterValue_, shift, 24, 1);
//}
//
//
////==============================================================================
// void VIPICFirmware::shiftBufferConfigurationCLK(bool shift)
//{
//    //Bit 26  . . . . Write 1 to invoke the state machine that shifts the D
//    //It loads the D buffer to the chip
//    BitManipulator::insertBits(pixelClockRegisterValue_, shift, 24, 1);
//}
//
////==============================================================================
// void VIPICFirmware::assertLoadCLK(bool load)
//{
//    //Bit 27  . . . . Asserts the load signal for one serial_clk cycle.
//    BitManipulator::insertBits(pixelClockRegisterValue_, load, 27, 1);
//}
//
////==============================================================================
// void VIPICFirmware::assertRestartCLK(bool restart)
//{
//    //Bit 29  . . . . Asserts the restart signal for one serial_clk cycle.
//    BitManipulator::insertBits(pixelClockRegisterValue_, restart, 29, 1);
//}
//
//
////==============================================================================
// void VIPICFirmware::stepTimeStampCLK(bool shift)
//{
//    //Bit 30  . . . . Send one ts_clk pulse
//    BitManipulator::insertBits(pixelClockRegisterValue_, shift, 30, 1);
//}
//
////==============================================================================
// void VIPICFirmware::stepReadCycleCLK(bool shift)
//{
//    //Bit 31  . . . . Sends one ready cycle
//    BitManipulator::insertBits(pixelClockRegisterValue_, shift, 31, 1);
//}
//
//
////TODO: waiting for documentation for LINK, ERROR_INJECT, SPY, STREAMER, RESET
//
///*
// * Pixels mask functions
// */
//
////==============================================================================
// int VIPICFirmware::pixelIndex(int column, int row)
//{
//    //Returns the index of the selected pixel
//    //FIXME: It is still to be decided if the sorting is forward or reverse
//    assert(column >= 0 && column < PIXELS_SIDE);
//    assert(row >= 0 && row < PIXELS_SIDE);
//    return PIXELS_SIDE*PIXELS_SIDE - (row | (column << 6));
//}
//
////==============================================================================
// int VIPICFirmware::pixelColumn(int index)
//{
//    //Returns the column coordinate of the selected pixel
//    //FIXME: It is still to be decided if the sorting is forward or reverse
//    assert(index >= 0 && index < PIXELS_SIDE * PIXELS_SIDE);
//    return 0x3f & ((PIXELS_SIDE*PIXELS_SIDE - index) >> 6);
//}
//
////==============================================================================
// int VIPICFirmware::pixelRow(int index)
//{
//    //Returns the row coordinate of the selected pixel
//    //FIXME: It is still to be decided if the sorting is forward or reverse
//    assert(index >= 0 && index < PIXELS_SIDE * PIXELS_SIDE);
//    return 0x3f & (PIXELS_SIDE*PIXELS_SIDE - index);
//}
//
////==============================================================================
// void VIPICFirmware::maskSetPixel(int index, bool force)
//{
//    //Forces a pixel to read without hit
//    BitManipulator::insertBits(setBuffer_[index / 32], force, index % 32, 1);
//}
//
////==============================================================================
// void VIPICFirmware::maskOffPixel(int index, bool kill)
//{
//    //Masks a pixel
//    BitManipulator::insertBits(resetBuffer_[index / 32], kill, index % 32, 1);
//}
//
////==============================================================================
// void VIPICFirmware::configurePixel(int index, uint16_t configure)
//{
//    //Configures a pixel
//    BitManipulator::insertBits(configureBuffer_[index / 2], configure, 16 * (index % 2),
//    12);
//}
//
////==============================================================================
// void VIPICFirmware::maskSetPixel(int column, int row, bool force)
//{
//    //Forces a pixel to read without hit
//    maskSetPixel(pixelIndex(column, row), force);
//}
//
////==============================================================================
// void VIPICFirmware::maskOffPixel(int column, int row, bool kill)
//{
//    //Masks a pixel
//    maskOffPixel(pixelIndex(column, row), kill);
//}
//
////==============================================================================
// void VIPICFirmware::configurePixel(int column, int row, uint16_t configure)
//{
//    //Configures a pixel
//    configurePixel(pixelIndex(column, row), configure);
//}
//
////==============================================================================
// bool VIPICFirmware::getSetMaskPixel(int column, int row)
//{
//    //Must be used after the load of the buffer from the device to seek for errors in
//    the chain int index = pixelIndex(column, row); return
//    BitManipulator::readBits(setBuffer_[index / 32], index % 32, 1);
//}
//
////==============================================================================
// bool VIPICFirmware::getResetMaskPixel(int column, int row)
//{
//    //Must be used after the load of the buffer from the device to seek for errors in
//    the chain int index = pixelIndex(column, row); return
//    BitManipulator::readBits(resetBuffer_[index / 32], index % 32, 1);
//}
//
////==============================================================================
// bool VIPICFirmware::getConfigurationPixel(int column, int row)
//{
//    //Must be used after the load of the buffer from the device to seek for errors in
//    the chain int index = pixelIndex(column, row); return
//    BitManipulator::readBits(resetBuffer_[index / 2], 16 * (index % 2), 12);
//}
//
