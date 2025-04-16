#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/PurdueFirmwareCore.h"
#include "otsdaq-fermilabtestbeam/DAQFirmwareHardware/FirmwareBaseDefinitions.h"
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq/MessageFacility/MessageFacility.h"
#include "otsdaq/BitManipulator/BitManipulator.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>

using namespace ots;

//==============================================================================
PurdueFirmwareCore::PurdueFirmwareCore(unsigned int version)
    : FrontEndFirmwareBase(version)
{
}

//==============================================================================
PurdueFirmwareCore::~PurdueFirmwareCore(void) {}

//==============================================================================
void PurdueFirmwareCore::init(void) {}

//==============================================================================
void PurdueFirmwareCore::setDataDestination(std::string&       buffer,
                                            const std::string& ipAddress,
                                            const uint16_t     port,
                                            bool               clearBuffer)
{
	// do nothing

	if(clearBuffer)
		buffer.resize(0);
}

//==============================================================================
std::string PurdueFirmwareCore::read(char* address)
{
	std::string buffer;
	read(buffer, *(uint32_t*)address);
	return buffer;
}

//==============================================================================
std::string PurdueFirmwareCore::write(char* address, char* data)
{
	std::string buffer;
	write(buffer, *(uint32_t*)address, *(uint32_t*)data);
	return buffer;
}

//==============================================================================
void PurdueFirmwareCore::write(std::string& buffer,
                               uint32_t     address,
                               uint32_t     data,
                               bool         clearBuffer)
{
	if(clearBuffer)
		buffer.resize(0);

	// std::cout << __COUT_HDR_FL__ << "Making buffer" << std::endl;
	unsigned int begin                    = buffer.length();
	unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
	buffer.resize(buffer.length() + 12, '\0');
	if(begin != 0)
		begin -= 1;
	buffer[begin + 0]  = numberOfBufferedCommands + 1;
	buffer[begin + 1]  = WRITE;
	buffer[begin + 2]  = 0;
	buffer[begin + 3]  = 8;
	buffer[begin + 4]  = (address >> 24) & 0xff;
	buffer[begin + 5]  = (address >> 16) & 0xff;
	buffer[begin + 6]  = (address >> 8) & 0xff;
	buffer[begin + 7]  = address & 0xff;
	buffer[begin + 8]  = (data >> 24) & 0xff;
	buffer[begin + 9]  = (data >> 16) & 0xff;
	buffer[begin + 10] = (data >> 8) & 0xff;
	buffer[begin + 11] = data & 0xff;
	if(begin == 0)
		buffer += '\0';
	// return (unsigned int) buffer[begin] + 1;
}

//==============================================================================
void PurdueFirmwareCore::waitSet(std::string& buffer,
                                 uint32_t     address,
                                 uint32_t     data,
                                 uint32_t     timeout,
                                 bool         clearBuffer)
{
	if(clearBuffer)
		buffer.resize(0);

	unsigned int begin                    = buffer.length();
	unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
	buffer.resize(buffer.length() + 16, '\0');
	if(begin != 0)
		begin -= 1;
	buffer[begin + 0]  = numberOfBufferedCommands + 1;
	buffer[begin + 1]  = WAITSET;
	buffer[begin + 2]  = 0;
	buffer[begin + 3]  = 12;
	buffer[begin + 4]  = (address >> 24) & 0xff;
	buffer[begin + 5]  = (address >> 16) & 0xff;
	buffer[begin + 6]  = (address >> 8) & 0xff;
	buffer[begin + 7]  = address & 0xff;
	buffer[begin + 8]  = (data >> 24) & 0xff;
	buffer[begin + 9]  = (data >> 16) & 0xff;
	buffer[begin + 10] = (data >> 8) & 0xff;
	buffer[begin + 11] = data & 0xff;
	buffer[begin + 12] = (timeout >> 24) & 0xff;
	buffer[begin + 13] = (timeout >> 16) & 0xff;
	buffer[begin + 14] = (timeout >> 8) & 0xff;
	buffer[begin + 15] = timeout & 0xff;
	if(begin == 0)
		buffer += '\0';
	// return (unsigned int) buffer[begin] + 1;
}

//==============================================================================
void PurdueFirmwareCore::waitClear(std::string& buffer,
                                   uint32_t     address,
                                   uint32_t     data,
                                   uint32_t     timeout,
                                   bool         clearBuffer)
{
	if(clearBuffer)
		buffer.resize(0);

	unsigned int begin                    = buffer.length();
	unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
	buffer.resize(buffer.length() + 16, '\0');
	if(begin != 0)
		begin -= 1;
	buffer[begin + 0]  = numberOfBufferedCommands + 1;
	buffer[begin + 1]  = WAITCLR;
	buffer[begin + 2]  = 0;
	buffer[begin + 3]  = 12;
	buffer[begin + 4]  = (address >> 24) & 0xff;
	buffer[begin + 5]  = (address >> 16) & 0xff;
	buffer[begin + 6]  = (address >> 8) & 0xff;
	buffer[begin + 7]  = address & 0xff;
	buffer[begin + 8]  = (data >> 24) & 0xff;
	buffer[begin + 9]  = (data >> 16) & 0xff;
	buffer[begin + 10] = (data >> 8) & 0xff;
	buffer[begin + 11] = data & 0xff;
	buffer[begin + 12] = (timeout >> 24) & 0xff;
	buffer[begin + 13] = (timeout >> 16) & 0xff;
	buffer[begin + 14] = (timeout >> 8) & 0xff;
	buffer[begin + 15] = timeout & 0xff;
	if(begin == 0)
		buffer += '\0';
	// return (unsigned int) buffer[begin] + 1;
}

//==============================================================================
void PurdueFirmwareCore::read(std::string& buffer, uint32_t address, bool clearBuffer)
{
	if(clearBuffer)
		buffer.resize(0);

	unsigned int begin                    = buffer.length();
	unsigned int numberOfBufferedCommands = getNumberOfBufferedCommands(buffer);
	buffer.resize(buffer.length() + 8, '\0');
	if(begin != 0)
		begin -= 1;
	buffer[begin + 0] = numberOfBufferedCommands + 1;
	buffer[begin + 1] = READ;
	buffer[begin + 2] = 0;
	buffer[begin + 3] = 4;
	buffer[begin + 4] = (address >> 24) & 0xff;
	buffer[begin + 5] = (address >> 16) & 0xff;
	buffer[begin + 6] = (address >> 8) & 0xff;
	buffer[begin + 7] = address & 0xff;
	if(begin == 0)
		buffer += '\0';
	// return (unsigned int) buffer[begin] + 1;
}

//==============================================================================
unsigned int PurdueFirmwareCore::getNumberOfBufferedCommands(std::string& buffer)
{
	if(buffer.length() == 0)
		return 0;
	unsigned int bufferPosition = 0;
	unsigned int commandNumber  = 0;
	while(bufferPosition < buffer.length() - 1)
	{
		bufferPosition += (unsigned int)buffer[bufferPosition + 3] + 4;
		++commandNumber;
	}
	return commandNumber;
}

//==============================================================================
std::string PurdueFirmwareCore::compareSendAndReceive(const std::string& sentBuffer,
                                                      std::string&       acknowledgment)
{
	std::string  reSendBuffer;
	unsigned int skipBuffer = 0;
	unsigned int skipAcknow = 0;
	unsigned int remainBufferCommands;
	unsigned int remainAcknowCommands;

	std::cout << __COUT_HDR_FL__ << "Comparing sent buffer and received acknowledgment!!!"
	          << std::endl;

	std::cout << __COUT_HDR_FL__ << "Buffer size (send): " << sentBuffer.size()
	          << std::endl;
	std::cout << __COUT_HDR_FL__
	          << "Acknowledgment size (receive): " << acknowledgment.size() << std::endl;

	while(skipBuffer < sentBuffer.size())
	{
		for(int position = 0; position < 4; position++)
		{
			if(position + skipBuffer >=
			   sentBuffer.size())  // make sure the number NEVER goes above the limit
			{
				reSendBuffer.clear();
				std::cout << __COUT_HDR_FL__ << "Done... Works!!!" << std::endl;
				return reSendBuffer;
			}

			if(position == 0)
			{
				if(sentBuffer[position + skipBuffer] !=
				   acknowledgment[position + skipAcknow])
				{
					std::cout << __COUT_HDR_FL__ << "ERROR - Sent "
					          << position + skipBuffer << "th: " << std::hex
					          << (unsigned int)sentBuffer[position + skipBuffer]
					          << std::dec << " different from Received "
					          << position + skipAcknow << "th:" << std::hex
					          << (unsigned int)acknowledgment[position + skipAcknow]
					          << std::dec;
					std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl;
					reSendBuffer.clear();
					for(unsigned int i = skipBuffer;
					    i < (skipBuffer + 4 + sentBuffer[skipBuffer + 3]);
					    i++)
					{
						reSendBuffer += sentBuffer[i];
					}
					return reSendBuffer;
				}
			}

			if(position == 1)
			{
				if((sentBuffer[position + skipBuffer]) !=
				   (acknowledgment[position + skipAcknow] & 0xf))
				{
					std::cout << __COUT_HDR_FL__ << "ERROR - Sent "
					          << position + skipBuffer << "th: " << std::hex
					          << (unsigned int)sentBuffer[position + skipBuffer]
					          << std::dec << " different from Received "
					          << position + skipAcknow << "th:" << std::hex
					          << (unsigned int)acknowledgment[position + skipAcknow]
					          << std::dec;
					std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl;
					reSendBuffer.clear();
					for(unsigned int i = skipBuffer;
					    i < (skipBuffer + 4 + sentBuffer[skipBuffer + 3]);
					    i++)
					{
						reSendBuffer += sentBuffer[i];
					}
					return reSendBuffer;
				}
			}

			if(position == 2)
			{
				if((sentBuffer[position + skipBuffer] != 0) ||
				   (acknowledgment[position + skipAcknow] != 0))
				{
					std::cout << __COUT_HDR_FL__ << "ERROR - Sent "
					          << position + skipBuffer << "th: " << std::hex
					          << (unsigned int)sentBuffer[position + skipBuffer]
					          << std::dec << " different from Received "
					          << position + skipAcknow << "th:" << std::hex
					          << (unsigned int)acknowledgment[position + skipAcknow]
					          << std::dec << std::endl;
					std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl;
					reSendBuffer.clear();
					for(unsigned int i = skipBuffer;
					    i < (skipBuffer + 4 + sentBuffer[skipBuffer + 3]);
					    i++)
					{
						reSendBuffer += sentBuffer[i];
					}
					return reSendBuffer;
				}
			}

			if(position == 3)
			{
				remainBufferCommands =
				    sentBuffer[position +
				               skipBuffer];  // 4th bit tells us how many bits are left
				remainAcknowCommands = acknowledgment[position + skipAcknow];

				if(((remainBufferCommands == 12) && (remainAcknowCommands == 4)) ||
				   ((remainBufferCommands == 8) && (remainAcknowCommands == 0)))
				{
					skipBuffer +=
					    (4 + remainBufferCommands);  // go to the beginning of next buffer
					skipAcknow += (4 + remainAcknowCommands);
				}
				else
				{
					std::cout << __COUT_HDR_FL__ << "ERROR - Sent "
					          << position + skipBuffer << "th: " << std::hex
					          << (unsigned int)sentBuffer[position + skipBuffer]
					          << std::dec << " not compatible with Received "
					          << position + skipAcknow << "th:" << std::hex
					          << (unsigned int)acknowledgment[position + skipAcknow]
					          << std::dec << std::endl;
					std::cout << __COUT_HDR_FL__ << "Position: " << position << std::endl;
					reSendBuffer.clear();
					for(unsigned int i = skipBuffer;
					    i < (skipBuffer + 4 + sentBuffer[skipBuffer + 3]);
					    i++)
					{
						reSendBuffer += sentBuffer[i];
					}
					return reSendBuffer;
				}
			}
		}
	}

	reSendBuffer.clear();
	std::cout << __COUT_HDR_FL__ << "Done... Works!!!" << std::endl;
	return reSendBuffer;
}

//==============================================================================
uint32_t PurdueFirmwareCore::createRegisterFromValue(std::string& readBuffer,
                                                     std::string& receivedValue)
{
	for(int position = 0; position < 8; position++)
	{
		if(position == 0 || position == 4 || position == 5 || position == 6 ||
		   position == 7)
		{
			if(readBuffer[position] != receivedValue[position])
			{
				std::cout << __COUT_HDR_FL__ << "ERROR - SentBuffer position " << position
				          << " value: " << std::hex << (unsigned int)readBuffer[position]
				          << std::dec << " different from ReceivedBuffer position "
				          << position << " value: " << std::hex
				          << (unsigned int)receivedValue[position] << std::dec
				          << std::endl;
				return 0;
			}
		}

		if(position == 1)
		{
			if((readBuffer[position] != (receivedValue[position] & 0xf)))
			{
				std::cout << __COUT_HDR_FL__ << "ERROR - SentBuffer position " << position
				          << " value: " << std::hex << (unsigned int)readBuffer[position]
				          << std::dec << " different from ReceivedBuffer position "
				          << position << " value: " << std::hex
				          << (unsigned int)receivedValue[position] << std::dec
				          << std::endl;
				return 0;
			}
		}

		if(position == 2)
		{
			if((readBuffer[position] != 0) || (receivedValue[position] != 0))
			{
				std::cout << __COUT_HDR_FL__ << "ERROR - SentBuffer position " << position
				          << " value: " << std::hex << (unsigned int)readBuffer[position]
				          << std::dec << " different from ReceivedBuffer position "
				          << position << " value: " << std::hex
				          << (unsigned int)receivedValue[position] << std::dec
				          << std::endl;
				return 0;
			}
		}

		if(position == 3)
		{
			if((readBuffer[position] != 4) || (receivedValue[position] != 8))
			{
				std::cout << __COUT_HDR_FL__ << "ERROR - SentBuffer position " << position
				          << " value: " << std::hex << (unsigned int)readBuffer[position]
				          << std::dec << " different from ReceivedBuffer position "
				          << position << " value: " << std::hex
				          << (unsigned int)receivedValue[position] << std::dec
				          << std::endl;
				return 0;
			}
		}
	}

	uint32_t registerValue;

	registerValue =
	    ((unsigned int)receivedValue[8] << 24) + ((unsigned int)receivedValue[9] << 16) +
	    ((unsigned int)receivedValue[10] << 8) + (unsigned int)receivedValue[11];

	std::cout << __COUT_HDR_FL__ << "No problem encountered! Register value created!!! "
	          << std::endl;

	return registerValue;
}
