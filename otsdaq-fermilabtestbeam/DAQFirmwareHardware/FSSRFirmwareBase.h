#ifndef _ots_FSSRFirmware_h_
#define _ots_FSSRFirmware_h_

#include <string>


#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/FirmwareSequence.h" 	/* for FirmwareSequence */
#include "otsdaq-fermilabtestbeam/DetectorConfiguration/ROCStream.h"	/* for ROCStream */

//#include "otsdaq-fermilabtestbeam/DAQHardware/PurdueFirmwareCore.h"


namespace ots
{

//		- FrontEndFirmwareBase (all virtual read, write, setDest, init) *functions are not pure virtual, rather throw exceptions if not overridden to allow setDest to go unused, for example.
//		- OtsUDPFirmwareCore (inherits and implements FrontEndFirmwareBase)
//		- PurdueFirmwareCore (inherits and implements FrontEndFirmwareBase)
//
//		- FSSRFirmwareBase (takes ONLY parameter for communication type. Does not inherit from anything). *Implements 90-100% base fssr functionality:
//		- FSSROtsFirmware (inherits from FSSRApplicationFirmwareBase only and overrides any 10% special functions)
//		- FSSRPurdueFirmware (inherits from FSSRApplicationFirmwareBase only and overrides any 10% special functions)
//					FSSRFirmwareBase::setCSR()
//					{
//					Communication->write(CSR);
//					}
//
//		- At PLUGIN level (takes parameter for communication AND application firmware type.  ALSO -PRESTON WAS partially RIGHT - inherit from FrontEndFirmwareBase AND FSSRFirmwareBase. ) * implement all the base functions using the pointers:
//
//					PLUGIN::PLUGIN(commType,appType)
//					{
//						If(appType == otsApp)
//						 FSSFirmware = FSSROtsFirmware(commType); //IMPORTANT pass comm type
//						Else
//						 FSSFirmware = FSSRPurdueFirmware(commType); //IMPORTANT pass comm type
//					}
//
//					PLUGIN::whatever()
//					{
//						FSSFirmware->read();
//						FSSFirmware->setCSR(); //will call override version if implemented
//					}
//
//		* This way the FE interface plugin code is exactly the same for both cases, just modified by type strings.
//		* Since only modified by type strings, this means there only needs to be one FE plugin class! Types controlled by configuration.

class FrontEndFirmwareBase;

class FSSRFirmwareBase
{

public:
    FSSRFirmwareBase 										(const std::string& communicationFirmwareType, unsigned int communicationFirmwareVersion = -1, unsigned int version = -1);
    virtual ~FSSRFirmwareBase								(void);
    void 					init							(void);

    std::string 			universalRead	  				(char* address);
    std::string 			universalWrite	  				(char* address, char* data);

    //virtual void  		    setDataDestination              (std::string& buffer, const std::string& ip, const uint16_t port);//					{__SS__; __THROW__(ss.str() + "Illegal call to undefined base class member function"); return;};
    uint32_t 				createRegisterFromValue 		(std::string& readBuffer, std::string& receivedValue);

    //FEW specific methods
    std::string 			configureClocks     			(std::string source, double frequency);
    std::string 			resetDetector       			(int channel=-1);
    std::string 			enableTrigger       			(void);
    //virtual void setDataDestination (std::string& buffer, const std::string& ip, const uint16_t port);
    //std::string setDataDestination  (std::string ip, uint32_t port);

    void 					resetDCM                   		(std::string& buffer);
    //only the first 5 bits of the uint8_t are used. Bit 0 correspond to chip 0, bit 1 to chip 1.... 
    void 			        alignReadOut         	        (std::string& buffer, uint8_t channel0, uint8_t channel1, uint8_t channel2, uint8_t channel3, uint8_t channel4, uint8_t channel5);
    void 					makeDACSequence 				(FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream& rocStream);
    void 					makeDACSequence 				(FirmwareSequence<uint32_t>& sequence, unsigned int channel, const ROCStream& rocStream);
    void 					makeMaskSequence				(FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream& rocStream);
    void 					makeMaskSequence				(FirmwareSequence<uint32_t>& sequence, unsigned int channel, const ROCStream& rocStream);

    void 					makeDACBuffer   				(std::string& buffer, unsigned int channel, const ROCStream& rocStream);
    void 					makeDACBuffer   				(std::vector<std::string>& buffer, unsigned int channel, const ROCStream& rocStream);
    void 					makeMaskBuffer  				(std::string& buffer, unsigned int channel, const ROCStream& rocStream);
    void 					setFrequencyFromClockState		(std::string& buffer, double frequency);
    bool 					isClockStateExternal			(void);

    std::string 			readFromAddress					(uint64_t address);

    //FER specific methods
    std::string 			resetBCO            	(void);
    std::string 			armBCOReset         	(void);
    std::string 			startStream         	(bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);
    std::string 			stopStream          	(void);


    void 					makeMaskSequence		(FirmwareSequence<uint64_t>& sequence, unsigned int channel, const ROCStream& rocStream, const std::string& registerName);
    void 					makeMaskSequence		(FirmwareSequence<uint32_t>& sequence, unsigned int channel, const ROCStream& rocStream, const std::string& registerName);
    void 					makeMaskBuffer  		(std::string& buffer, unsigned int channel, const ROCStream& rocStream, const std::string& registerName);

    void 					setCSRRegister			(uint32_t total);

    //Set values for register STRIP_CSR (Strip Control Register)
    //STRIP_CSR = 0xc4000000
    //Byte 4-------------------
    //31    DCM_RESET
    //30    BCO_LOCKED
    //29    MCCLK_LOCKED
    //28    unused
    ///////////////////////////
    //27    STREAM_ENABLE
    //26 	SEND_BCO
    //25    SEND_TRIG_NUM
    //24    SEND_TRIG
    //Byte 3-------------------
    //23    TRIG_ENABLE
    //22    BCO_CLEAR
    //21    TRIG_NUM_CLEAR
    //20    FLUSH
    ///////////////////////////
    //19    ARM_BCO_RESET
    //18-17 unused
    //16    EXT_BCO
    //Byte 2-------------------
    //15-14 IDLE_COUNT
    //13-8  MODULE_ENABLE
    //Byte 1-------------------
    //7-3   PACKET_SIZE
    //2-0   N_CHANNELS

    uint32_t stripCSRRegisterValue_;
    void 					setPacketSizeStripCSR	         (uint32_t size);
    void 					enableChannelsStripCSR           (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);
    void 					setExternalBCOClockSourceStripCSR(std::string clockSource);
    //void 					setHaltStripCSR                  (bool set);//2018-10-24 Doesn't exist????
    void 					armBCOResetCSR                   (void);
	void 					flushBuffersStripCSR             (void);
	void 					resetTriggerCounterStripCSR      (std::string& buffer);
	void 					resetBCOCounterStripCSR          (void);
	void 					enableTriggerStripCSR            (bool enable);
	void 					sendTriggerDataStripCSR          (bool send);
	void 					sendTriggerNumberStripCSR        (bool send);
	void 					sendBCOStripCSR                  (bool send);
	void 					enableStreamStripCSR             (bool enable);
	void 					resetDCMStripCSR                 (bool clear);
	uint32_t 				waitDCMResetStripCSR             (void);
	std::string 			readCSRRegister			         (void);

	std::string 			readSCCSRRegister   	         (void);
	std::string 			readTrimCSRRegister              (void);

    uint32_t stripResetRegisterValue_;
    //Set values for register STRIP_RESET (Strip Reset)
    void resetStripResetRegisterValue(void)
    {
        stripResetRegisterValue_ = 0;
    }
    void 					resetDAC                    (void);
    void 					resetLink                   (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);
    void 					clearErrors                 (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);
    void 					clearFIFO                   (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);
    void 					resetChip                   (bool channel0, bool channel1, bool channel2, bool channel3, bool channel4, bool channel5);

    uint32_t stripBCODCMRegisterValue_;
    void 					setFrequencyRatio           (std::string& buffer, int numerator, int denominator);

    void 					configureStripTriggerUnbiased		(std::string& buffer);
    void 					configureTriggerInputs				(std::string& buffer);

    //STRIP_TRIG_CSR = 0xc40000060
	uint32_t stripTriggerCSRRegisterValue_;

	//Registers setters
	void 					BCOOffset					(uint32_t offset);
	void 					selectSpyFIFO				(uint32_t input);
	void 					halt						(bool halt);


	std::string  			resetSlaveBCO(void);

    static const std::string PURDUE_FIRMWARE_NAME;
    static const std::string OTS_FIRMWARE_NAME;

    FrontEndFirmwareBase* 	communicationFirmwareInstance_;

protected:
    unsigned int version_;
    const std::string communicationFirmwareType_;
};

}

#endif
