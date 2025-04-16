#ifndef _ots_VIPICFirmwareDefinitions_h_
#define _ots_VIPICFirmwareDefinitions_h_

#include <stdint.h>

namespace ots
{
const uint32_t ETHIO_DESTINATION_PORT       = 0xc1000018;
const uint32_t DATA_DESTINATION_IP          = 0xc10000a4;
const uint32_t DATA_SOURCE_DESTINATION_PORT = 0xc10000a8;

const double EXTERNAL_CLOCK_FREQUENCY = 54;      // base freq: 54MHz
const double INTERNAL_CLOCK_FREQUENCY = 66.667;  // base freq: 66.667MHz

const uint8_t WRITE   = 1;
const uint8_t SET     = 2;
const uint8_t READ    = 4;
const uint8_t RESET   = 5;
const uint8_t CLEAR   = 5;
const uint8_t DEFAULT = 6;
const uint8_t WAITCLR = 7;
const uint8_t WAITSET = 8;

const int PIXELS_SIDE = 64;

const unsigned int VIPIC_CLOCK       = 0xc9000004;  //  Still undefined
const unsigned int VIPIC_CSR         = 0xc9000000;  //  Command/status register
const unsigned int VIPIC_LINK        = 0xc9000010;  //  Serial link status
const unsigned int VIPIC_TEST_INJECT = 0xc9000020;  //  Drives test data on connector J3
const unsigned int VIPIC_SPY_FIFO_STATUS = 0xc9000024;  //  Spy FIFO status register
const unsigned int VIPIC_SPY_FIFO_DATA   = 0xc9000028;  //  Spy FIFO data register
const unsigned int VIPIC_TS_COUNTER_LOW  = 0xc9000040;  //  TS counter low 32 bits
const unsigned int VIPIC_TS_COUNTER_HIGH = 0xc9000044;  //  TS counter high 16 bits
const unsigned int VIPIC_DCM_DATA = 0xc9000048;  //  DCM configuration (old BCO data)
const unsigned int VIPIC_STREAMER = 0xc900004c;  //  Data stream status register

const unsigned int VIPIC_SET_WRITE_BASE   = 0xc9001000;  //  Set write buffer
const unsigned int VIPIC_SET_WRITE_END    = 0xc90011ff;  //
const unsigned int VIPIC_SET_READ_BASE    = 0xc9001200;  //  Set readback buffer
const unsigned int VIPIC_SET_READ_END     = 0xc90013ff;  //
const unsigned int VIPIC_RESET_WRITE_BASE = 0xc9001400;  //  Reset write buffer
const unsigned int VIPIC_RESET_WRITE_END  = 0xc90015ff;  //
const unsigned int VIPIC_RESET_READ_BASE  = 0xc9001600;  //  Reset readback buffer
const unsigned int VIPIC_RESET_READ_END   = 0xc90017ff;  //
const unsigned int VIPIC_DATA_WRITE_BASE  = 0xc9002000;  //  Data write buffer
const unsigned int VIPIC_DATA_WRITE_END   = 0xc90037ff;  //
const unsigned int VIPIC_DATA_READ_BASE   = 0xc9004000;  //  Data readback buffer
const unsigned int VIPIC_DATA_READ_END    = 0xc90057ff;  //
}

#endif
