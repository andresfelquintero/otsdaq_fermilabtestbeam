//#ifndef _ots_PSI46DigFirmware_h_
//#define _ots_PSI46DigFirmware_h_
//
//#include "otsdaq-components/DAQHardware/FrontEndFirmwareBase.h"
//#include "otsdaq-components/DAQHardware/PurdueFirmwareCore.h"
//
//#include <string>
//
// namespace ots
//{
//
// class PSI46DigFirmware : public FrontEndFirmwareBase/*, public PurdueFirmwareCore*/
//{
//
// public:
//    PSI46DigFirmware (unsigned int version,std::string type);
//    virtual ~PSI46DigFirmware(void);
//    int  init(void);
//    //FEW specific methods
//    std::string configureClocks(std::string source, double frequency);
//    std::string resetDetector  (int channel=-1);
//    std::string enableTrigger  (void);
//
//    void makeDACSequence 				(FirmwareSequence<uint64_t>& sequence, unsigned
//    int channel, const ROCStream& rocStream); void makeMaskSequence
//    (FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream&
//    rocStream); void makeDACSequence 				(FirmwareSequence<uint32_t>& sequence,
//    unsigned int channel, const ROCStream& rocStream); void makeDACBuffer
//    (std::string& buffer, unsigned int channel, const ROCStream& rocStream); void
//    makeDACBuffer   				(std::vector<std::string>& buffer, unsigned int
//    channel, const ROCStream& rocStream); void makeMaskSequence
//    (FirmwareSequence<uint32_t>& sequence, unsigned int channel, const ROCStream&
//    rocStream); void makeMaskBuffer (std::string& buffer, unsigned int channel, const
//    ROCStream& rocStream); void setFrequencyFromClockState		        (std::string&
//    buffer, double frequency); bool isClockStateExternal			        (void);
//
//    //FER specific methods
//    std::string resetBCO           (void);
//    std::string startStream        (bool channel0, bool channel1, bool channel2, bool
//    channel3, bool channel4, bool channel5); std::string stopStream         (void);
//
// protected:
//    FrontEndFirmwareBase* protocolInstance_;
//    void makeMaskSequence(FirmwareSequence<uint64_t>& sequence, unsigned int channel,
//    const ROCStream& rocStream, const std::string& registerName); void
//    makeMaskSequence(FirmwareSequence<uint32_t>& sequence, unsigned int channel, const
//    ROCStream& rocStream, const std::string& registerName); void makeMaskBuffer
//    (std::string& buffer, unsigned int channel, const ROCStream& rocStream, const
//    std::string& registerName);
//
//    void setCSRRegister						(uint32_t total);
//
//    uint32_t stripCSRRegisterValue_;
//    //Set values for register STRIP_CSR (Strip Control Register)
//    void setPacketSizeStripCSR			  (uint32_t size);
//    void enableChannelsStripCSR           (bool channel0, bool channel1, bool channel2,
//    bool channel3, bool channel4, bool channel5); void
//    setExternalBCOClockSourceStripCSR(std::string clockSource); void setHaltStripCSR
//    (bool set); void enableBCOStripCSR                (bool enable);
//	void flushBuffersStripCSR             (void);
//	void resetTriggerCounterStripCSR      (std::string& buffer);
//	void resetBCOCounterStripCSR          (void);
//	void enableTriggerStripCSR            (bool enable);
//	void sendTriggerDataStripCSR          (bool send);
//	void sendTriggerNumberStripCSR        (bool send);
//	void sendBCOStripCSR                  (bool send);
//	void enableStreamStripCSR             (bool enable);
//	void resetDCMStripCSR                 (bool clear);
//	uint32_t waitDCMResetStripCSR         (void);
//	std::string readCSRRegister			  (void);
//
//    uint32_t stripResetRegisterValue_;
//    //Set values for register STRIP_RESET (Strip Reset)
//    void resetStripResetRegisterValue(void)
//    {
//        stripResetRegisterValue_ = 0;
//    }
//    void resetDAC                    (void);
//    void resetLink                   (bool channel0, bool channel1, bool channel2, bool
//    channel3, bool channel4, bool channel5); void clearErrors                 (bool
//    channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool
//    channel5); void clearFIFO                   (bool channel0, bool channel1, bool
//    channel2, bool channel3, bool channel4, bool channel5); void resetChip
//    (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool
//    channel5);
//
//    uint32_t stripBCODCMRegisterValue_;
//    void setFrequencyRatio           (std::string& buffer, int numerator, int
//    denominator);
//
//    void configureStripTriggerUnbiased		(std::string& buffer);
//    void configureTriggerInputs				(std::string& buffer);
//
//    uint32_t stripTriggerCSRRegisterValue_;
//	//Registers setters
//	void BCOOffset(uint32_t offset);
//	void selectSpyFIFO(uint32_t input);
//	void halt(bool halt);
//
//};
//
//}
//
//#endif
//
//
//
//
//
////#ifndef _ots_PSI46DigFirmware_h
////#define _ots_PSI46DigFirmware_h
////
////#include "otsdaq/DAQHardware/FirmwareBase.h"
//////#include "otsdaq/DataTypes/DataTypes.h"
//////#include "otsdaq/ConfigurationInterface/ROCStream.h"
////
////#include <string>
//////#include <vector>
////
////namespace ots
////{
////
////class PSI46DigFirmware : public FirmwareBase
////{
////
////public:
////    PSI46DigFirmware (unsigned int version);
////    virtual ~PSI46DigFirmware(void);
////    int  init(ConfigurationManager *configManager);
////    //FEW specific methods
////    std::string configureClocks(std::string source, double frequency);
////    std::string resetDetector  (int channel=-1);
////    std::string enableTrigger  (void);
////
////    void makeDACSequence 				(FirmwareSequence<uint64_t>& sequence,
/// unsigned  int  channel, const ROCStream& rocStream); /    void makeMaskSequence
///(FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream&
/// rocStream);
////    void makeDACSequence 				(FirmwareSequence<uint32_t>& sequence,
/// unsigned  int  channel, const ROCStream& rocStream); /    void makeDACBuffer
/// (std::string& buffer,  unsigned int channel, const ROCStream& rocStream); /    void
/// makeDACBuffer
/// (std::vector<std::string>& buffer, unsigned int channel, const  ROCStream& rocStream);
////    void makeMaskSequence				(FirmwareSequence<uint32_t>& sequence,
/// unsigned  int  channel, const ROCStream& rocStream); /    void makeMaskBuffer
/// (std::string&  buffer, unsigned int channel, const ROCStream& rocStream); /
/// std::string compareSendAndReceive	(const std::string& sentBuffer, std::string&
/// acknowledgment); /    uint32_t createRegisterFromValue	(std::string& readBuffer,
/// std::string& receivedValue); /    void setFrequencyFromClockState		(std::string&
/// buffer, double inputedFrequency); /    bool isClockStateExternal			();
////
////    //FER specific methods
////    std::string setDataDestination(std::string ip, unsigned int port);
////    std::string resetBCO          (void);
////    std::string startStream       (bool channel0, bool channel1, bool channel2, bool
/// channel3, bool channel4, bool channel5); /    std::string stopStream        (void);
////
////protected:
////    unsigned int write       (std::string& buffer, unsigned int address, unsigned int
/// data); /    unsigned int waitClear   (std::string& buffer, unsigned int address,
/// unsigned int data, unsigned int=256); /    unsigned int waitSet     (std::string&
/// buffer, unsigned int address, unsigned int data, unsigned int=256); /    unsigned int
/// read        (std::string& buffer, unsigned int address); /    unsigned int
/// getNumberOfBufferedCommands(std::string& buffer); /    void
/// makeMaskSequence(FirmwareSequence<uint64_t>& sequence, unsigned int channel, const
/// ROCStream& rocStream, const std::string& registerName); /    void
/// makeMaskSequence(FirmwareSequence<uint32_t>& sequence, unsigned int channel, const
/// ROCStream& rocStream, const std::string& registerName); /    void makeMaskBuffer
///(std::string& buffer, unsigned int channel, const ROCStream& rocStream, const
/// std::string& registerName);
////
////    void setCSRRegister						(uint32_t total);
////
////    uint32_t stripCSRRegisterValue_;
////    //Set values for register STRIP_CSR (Strip Control Register)
////    void setPacketSizeStripCSR			  (uint32_t size);
////    void enableChannelsStripCSR           (bool channel0, bool channel1, bool
/// channel2, bool channel3, bool channel4, bool channel5); /    void
/// setExternalBCOClockSourceStripCSR(std::string clockSource); /    void
/// setFastBCOStripCSR               (bool set); /    void enableBCOStripCSR
///(bool enable); /	void flushBuffersStripCSR             (void); /	void
/// resetTriggerCounterStripCSR      (std::string& buffer);
////	void resetBCOCounterStripCSR          (void);
////	void enableTriggerStripCSR            (bool enable);
////	void sendTriggerDataStripCSR          (bool send);
////	void sendTriggerNumberStripCSR        (bool send);
////	void sendBCOStripCSR                  (bool send);
////	void enableStreamStripCSR             (bool enable);
////	void resetDCMStripCSR                 (bool clear);
////	void waitDCMResetStripCSR             (void);
////	std::string readCSRRegister			  (void);
////
////    uint32_t stripResetRegisterValue_;
////    //Set values for register STRIP_RESET (Strip Reset)
////    void resetStripResetRegisterValue(void)
////    {
////        stripResetRegisterValue_ = 0;
////    }
////    void resetDAC                    (void);
////    void resetLink                   (bool channel0, bool channel1, bool channel2,
/// bool channel3, bool channel4, bool channel5); /    void clearErrors
///(bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool
/// channel5); /    void clearFIFO                   (bool channel0, bool channel1, bool
/// channel2, bool channel3, bool channel4, bool channel5); /    void resetChip
///(bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool
/// channel5);
////
////    uint32_t stripBCODCMRegisterValue_;
////    void setFrequencyRatio           (std::string& buffer, int numerator, int
/// denominator);
////
////    void configureStripTriggerUnbiased		(std::string& buffer);
////    void configureTriggerInputs				(std::string& buffer);
////
////    uint32_t stripTriggerCSRRegisterValue_;
////	//Registers setters
////	void BCOOffset(uint32_t offset);
////	void selectSpyFIFO(uint32_t input);
////	void halt(bool halt);
////
////    /*
////    uint32_t stripSCRegisterValue_;
////    //Registers setters
////    void chipID(uint32_t size);
////    void addressSlowControls(uint32_t size);
////    void instructionSlowControls(uint32_t size);
////    void channelreadSelect(uint32_t size);
////    void channelMask(uint32_t size);
////    void bitsLength(uint32_t length);
////    void syncFallingBCO(bool sync);
////    void syncRisingBCO(bool syncpublic);
////    void setRaw(bool set);
////    void initSlowControls(bool init);
////
////    uint32_t stripAnalysisCSRRegisterValue_;
////    //Registers setters
////    void resetCount(bool reset);
////
////    uint32_t triggerInputRegisterValue_;
////    //Registers setters
////    void setBCO_0(uint32_t input);
////    void setBCO_1(uint32_t input);
////    void trimFracBCO_0(uint32_t input);
////    void trimFracBCO_1(uint32_t input);
////    void trimBCO(uint32_t input);
////    void enable_0(bool enable);
////    void enable_1(bool enable);
////	*/
////
////
////
////	/*
////	uint32_t stripTrimCSRRegisterValue_;
////	//Registers setters
////
////
////
////    uint32_t stripTriggerCSRRegisterValue_;
////    //Registers setters
////    void BCOOffset(uint32_t offset);
////    void selectSpyFIFO(uint32_t input);
////    void halt(bool halt);
////    */
////
////};
////
////}
////
////#endif
