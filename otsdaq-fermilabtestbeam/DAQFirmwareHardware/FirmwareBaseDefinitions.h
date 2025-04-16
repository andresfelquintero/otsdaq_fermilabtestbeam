#ifndef _ots_FirmwareBaseDefinitions_h
#define _ots_FirmwareBaseDefinitions_h

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
}

#endif
