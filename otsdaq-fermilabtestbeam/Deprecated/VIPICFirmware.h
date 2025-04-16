//#ifndef _ots_VIPICFirmware_h_
//#define _ots_VIPICFirmware_h_
//
//#include "otsdaq-components/DAQHardware/VIPICFirmwareDefinitions.h"
//#include "otsdaq-components/DAQHardware/FrontEndFirmwareBase.h"
////#include "otsdaq-components/DAQHardware/PurdueFirmwareCore.h"
//#include "otsdaq-components/DetectorConfiguration/ROCStream.h"
//
//#include <string>
//
// namespace ots
//{
//
// class VIPICFirmware : public FrontEndFirmwareBase/*, public PurdueFirmwareCore*/
//{
//
// private:
//    uint32_t pixelCSRRegisterValue_;           //CSR
//    uint32_t pixelDCMRegisterValue_;           //DCM
//    uint32_t pixelClockRegisterValue_;         //RegisterClock (does not handle the
//    clock, it is the name Matthew uses) uint32_t pixelTestInjectRegisterValue_;
//    //TestInject
//
//    uint32_t setBuffer_       [PIXELS_SIDE * PIXELS_SIDE / 32]; //S Buffer (one bit per
//    pixel) uint32_t resetBuffer_     [PIXELS_SIDE * PIXELS_SIDE / 32]; //R Buffer (one
//    bit per pixel) uint32_t configureBuffer_ [PIXELS_SIDE * PIXELS_SIDE / 2 ]; //D
//    Buffer (12 bits per pixel, extended to 16)
//
//
// public:
//    VIPICFirmware (unsigned int version, std::string type);
//    virtual ~VIPICFirmware();
//    int  init();
//
//    std::string resetTimeStamp              (void);
//    std::string configureClocks             (std::string source, double frequency);
//    std::string resetDetector               (void);
//    std::string enableTrigger               (bool enable);
//    std::string enableTrigger               (void){return enableTrigger(true);}
//    std::string resetDetector               (int reset);
//    std::string resetBCO                    (void);
//
//    void setFrequencyFromClockState         (std::string& buffer, double
//    inputedFrequency); bool isClockStateExternal               (void); bool
//    isClockLocked                      (void);
//
//    std::string resetTriggerCounter         (void);
//    std::string stopStream                  (void);
//    std::string startStream                 (void);
//    std::string setUp                       (void);
//
//    std::string prepareResetTimeStamp       (void);
//
//    void makeMaskBuffer                     (std::string& buffer, unsigned int channel,
//    const ROCStream& rocStream);
//
//
// protected:
//    FrontEndFirmwareBase* protocolInstance_;
//    void setTimeStampClockSourceCSR         (std::string clockSource);
//    void flushBuffersPixelCSR               (void);//FIXME:waiting for M
//
//    //Set values for Masks buffers
//    void maskSetPixel                       (int index, bool force);
//    void maskOffPixel                       (int index, bool kill);
//    void configurePixel                     (int index, uint16_t configure);
//    void maskSetPixel                       (int column, int row, bool force);
//    void maskOffPixel                       (int column, int row, bool kill);
//    void configurePixel                     (int column, int row, uint16_t configure);
//
//
//    bool getSetMaskPixel                    (int column, int row);
//    bool getResetMaskPixel                  (int column, int row);
//    bool getConfigurationPixel              (int column, int row);
//
//    //Set values for register PIXEL_DCM (Pixel Clock Manager)
//    void setTimeStampClockSource            (std::string clockSource);
//    void setFrequencyRatio                  (std::string& buffer, int numerator, int
//    denominator);
//
//    //Reset local register values
//    void resetPixelCSRRegisterValue         (void);
//    void resetPixelDCMRegisterValue         (void);
//    void resetPixelClockRegisterValue       (void);
//    void resetPixelTestInjectRegisterValue  (void);
//
//
//    //Register layer manipulation (must be issued a write after these functions)
//
//    //CSR
//    void setPacketSizePixelCSR              (uint32_t size);
//    void issueClearFIFOCSR                  (int delay);
//    void setTimeStampClockSourcePixelCSR    (bool externalClockSource);
//    void prepareResetTimeStampCSR           (void);
//    void enableTimeStampCounterPixelCSR     (bool enable);
//    void clearTriggerCounterPixelCSR        (void);
//    void immediateResetTimeStampCSR         (void);
//    void enableStreamPixelCSR               (bool enable);
//    void enableTriggerDataStreamPixelCSR    (bool enable);
//    void resetNetworkPixelCSR               (bool reset);
//    void resetDCMCSR                        (bool reset);
//    bool isClockLockedCSR                   (void);
//    bool isClockStateExternalCSR            (void);
//
//    //"Clock" Register
//    void waitTimeStampToReadCLK             (uint8_t delay);
//    void setReadsPerCycleCLK                (uint8_t readsPerCycle);
//    void setWaitingPerCycleCLK              (uint8_t delay);
//    void assertCleanEndCLK                  (bool clean);
//    void assertTimeStampEndCLK              (bool timestamp);
//    void shiftBufferSetCLK                  (bool shift);
//    void shiftBufferResetCLK                (bool shift);
//    void shiftBufferConfigurationCLK        (bool shift);
//    void assertLoadCLK                      (bool load);
//    void assertRestartCLK                   (bool restart);
//    void stepTimeStampCLK                   (bool shift);
//    void stepReadCycleCLK                   (bool shift);
//
//    //Mask functions
//
//    int pixelIndex                          (int column, int row);
//    int pixelColumn                         (int index);
//    int pixelRow                            (int index);
//
//};
//
//}
//
//#endif
