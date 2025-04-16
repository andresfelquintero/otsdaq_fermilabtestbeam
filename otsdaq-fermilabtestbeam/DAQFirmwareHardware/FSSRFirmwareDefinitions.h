#ifndef _ots_FSSRFirmwareDefinitions_h
#define _ots_FSSRFirmwareDefinitions_h

#include <stdint.h>

namespace ots
{
const uint64_t EnableSPIPixelNormalDataAddress = 0x300;
// const UInt64 ChannelFIFOAddress[2]           = {0x300,0x300};

const uint32_t STRIP_CSR                  = 0xc4000000;
const uint32_t STRIP_RESET                = 0xc4000004;
const uint32_t STRIP_SC_CSR               = 0xc4000008;  // Command/status register
const uint32_t STRIP_SCI                  = 0xc4000010;  // Write address for registers
const uint32_t STRIP_SCI0                 = 0xc4000010;  // Write address for mask bits
const uint32_t STRIP_SCI1                 = 0xc4000014;  // Write address for mask bits
const uint32_t STRIP_SCI2                 = 0xc4000018;  // Write address for mask bits
const uint32_t STRIP_SCI3                 = 0xc400001c;  // Write address for mask bits
const uint32_t STRIP_SCO                  = 0xc4000020;  // Read address for registers
const uint32_t STRIP_SCO0                 = 0xc4000020;  // Read address for mask bits
const uint32_t STRIP_SCO1                 = 0xc4000024;  // Read address for mask bits
const uint32_t STRIP_SCO2                 = 0xc4000028;  // Read address for mask bits
const uint32_t STRIP_SCO3                 = 0xc400002c;  // Read address for mask bits
const uint32_t STRIP_FIFO_CSR             = 0xc4000030;
const uint32_t STRIP_FIFO_DATA            = 0xc4000034;
const uint32_t STRIP_ANALYSIS_CSR         = 0xc4000038;
const uint32_t STRIP_ANALYSIS_BCO_COUNTER = 0xc400003c;
const uint32_t STRIP_ANALYSIS_CHIP1_HIT_COUNTER = 0xc400013c;
const uint32_t STRIP_ANALYSIS_CHIP2_HIT_COUNTER = 0xc400023c;
const uint32_t STRIP_ANALYSIS_CHIP3_HIT_COUNTER = 0xc400033c;
const uint32_t STRIP_ANALYSIS_CHIP4_HIT_COUNTER = 0xc400043c;
const uint32_t STRIP_ANALYSIS_CHIP5_HIT_COUNTER = 0xc400053c;
const uint32_t STRIP_CHIP1_STATUS               = 0xc4000130;
const uint32_t STRIP_CHIP2_STATUS               = 0xc4000230;
const uint32_t STRIP_CHIP3_STATUS               = 0xc4000330;
const uint32_t STRIP_CHIP4_STATUS               = 0xc4000430;
const uint32_t STRIP_CHIP5_STATUS               = 0xc4000530;
const uint32_t STRIP_BCO_COUNTER_LOW            = 0xc4000040;
const uint32_t STRIP_BCO_COUNTER_HIGH           = 0xc4000044;
const uint32_t STRIP_BCO_DCM                    = 0xc4000048;
// const uint32_t STRIP_STREAM_STATUS              = 0xc400004c;
const uint32_t STRIP_DAC_CSR   = 0xc4000050;
const uint32_t STRIP_DAC_INPUT = 0xc4000054;
const uint32_t STRIP_DAC_SPI   = 0xc4000058;
// const uint32_t STRIP_WORDS_DUMPED               = 0xc400005c;
const uint32_t STRIP_TRIG_CSR = 0xc4000060;
// const uint32_t STRIP_TRIG_FIFO_DATA             = 0xc4000064;
const uint32_t STRIP_TRIG_UNBIASED = 0xc4000068;
const uint32_t STRIP_TRIG_INPUT_0  = 0xc4000070;
const uint32_t STRIP_TRIG_INPUT_1  = 0xc4000074;
const uint32_t STRIP_TRIG_INPUT_2  = 0xc4000078;
const uint32_t STRIP_TRIG_INPUT_3  = 0xc400007c;
// const uint32_t STRIP_TRIGNUM_LOW                = 0xc4000080;
// const uint32_t STRIP_TRIGNUM_HIGH               = 0xc4000084;
const uint32_t ETHIO_DESTINATION_PORT       = 0xc1000018;
const uint32_t DATA_DESTINATION_IP          = 0xc10000a4;
const uint32_t DATA_SOURCE_DESTINATION_PORT = 0xc10000a8;

const uint32_t STRIP_TRIM_CSR = 0xc4000088;
const uint32_t STRIP_TLK_CSR  = 0xc3000000;

const double EXTERNAL_CLOCK_FREQUENCY = 54;      // base freq: 54MHz
const double INTERNAL_CLOCK_FREQUENCY = 66.667;  // base freq: 66.667MHz

const uint32_t ChannelFIFOAddress[6] = {
    STRIP_SCI, STRIP_SCI, STRIP_SCI, STRIP_SCI, STRIP_SCI, STRIP_SCI};

const uint8_t WRITE   = 1;
const uint8_t SET     = 2;
const uint8_t READ    = 4;
const uint8_t RESET   = 5;
const uint8_t CLEAR   = 5;
const uint8_t DEFAULT = 6;
const uint8_t WAITCLR = 7;
const uint8_t WAITSET = 8;

const uint8_t STIB_DAC_WRITE_MAX = 36;
}

#endif
