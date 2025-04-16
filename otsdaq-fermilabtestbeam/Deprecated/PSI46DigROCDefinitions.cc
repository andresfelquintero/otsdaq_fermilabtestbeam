/*
 * PSI46DigROCDefinitions.cc
 *
 *  Created on: Jun 12, 2013
 *      Author: Luigi Vigani
 */

#include "otsdaq-fermilabtestbeam/DetectorHardware/PSI46DigROCDefinitions.h"
#include "otsdaq/BitManipulator/BitManipulator.h"
//#include "otsdaq/MessageFacility/MessageFacility.h"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "otsdaq/Macros/CoutMacros.h"

using namespace ots;

//==============================================================================
PSI46DigROCDefinitions::PSI46DigROCDefinitions() {}

//==============================================================================
PSI46DigROCDefinitions::~PSI46DigROCDefinitions(void) {}

//==============================================================================
unsigned int PSI46DigROCDefinitions::getDACRegisterAddress(
    const std::string& registerName)
{
	if(registerName == "PulserData")
		return 1;
	else if(registerName == "PulserControl")
		return 2;
	else if(registerName == "IntegratorVbn")
		return 3;
	else if(registerName == "ShaperVbp2")
		return 4;
	else if(registerName == "ShaperVbp1")
		return 5;
	else if(registerName == "BLRestorer")
		return 6;
	else if(registerName == "VTn")
		return 7;
	else if(registerName == "VTp0")
		return 8;
	else if(registerName == "VTp1")
		return 9;
	else if(registerName == "VTp2")
		return 10;
	else if(registerName == "VTp3")
		return 11;
	else if(registerName == "VTp4")
		return 12;
	else if(registerName == "VTp5")
		return 13;
	else if(registerName == "VTp6")
		return 14;
	else if(registerName == "VTp7")
		return 15;
	else if(registerName == "ActiveLines")
		return 16;
	else if(registerName == "SendData")
		return 19;
	else if(registerName == "RejectHits")
		return 20;
	else if(registerName == "DigContrReg")
		return 27;
	else if(registerName == "AqBCO")
		return 30;
	// Kill and Inject are GENERAL names that will be used for all ROCs
	else if(registerName == "Kill")
		return 17;
	else if(registerName == "Inject")
		return 18;
	else
	{
		std::cout << __COUT_HDR_FL__ << " Register Name not recognized!" << std::endl;
		assert(0);
	}
	return 0;
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACWriteHeader(int                chipId,
                                                    const std::string& registerName)
{
	return makeDACHeader(chipId, registerName, write);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACSetHeader(int                chipId,
                                                  const std::string& registerName)
{
	return makeDACHeader(chipId, registerName, set);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACResetHeader(int                chipId,
                                                    const std::string& registerName)
{
	return makeDACHeader(chipId, registerName, reset);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACReadHeader(int                chipId,
                                                   const std::string& registerName)
{
	return makeDACHeader(chipId, registerName, read);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACHeader(int                chipId,
                                               const std::string& registerName,
                                               unsigned int       instruction)
{
	/*Ryan
	uint64_t command = 0;
	//Insert Chip ID
	BitManipulator::insertBits(command,BitManipulator::reverseBits(chipId,0,5),0,5);
	//Insert Register Address
	BitManipulator::insertBits(command,BitManipulator::reverseBits(getDACRegisterAddress(registerName),0,5),5,5);
	//Insert instruction
	BitManipulator::insertBits(command,BitManipulator::reverseBits(instruction,0,3),10,3);
*/
	uint32_t command         = 0x80000000;
	uint32_t registerAddress = getDACRegisterAddress(registerName);
	// Insert Chip ID
	BitManipulator::insertBits(command, chipId, 0, 5);
	// Insert Register Address
	BitManipulator::insertBits(command, registerAddress, 5, 5);
	// Insert instruction
	BitManipulator::insertBits(command, instruction, 10, 3);

	// Insert mask
	// BitManipulator::insertBits(command,0x1,16,8);
	unsigned int length = 3;
	if(registerAddress < 16 || registerAddress < 27 || registerAddress < 30)
		length = 3;
	else if(registerAddress == 17 || registerAddress == 18)
		length = 4;
	else if(registerAddress == 16)
		length = 2;
	else if(registerAddress == 19 || registerAddress == 20)
		length = 1;
	// Insert command length
	BitManipulator::insertBits(command, length, 24, 7);

	return command;
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACWriteCommand(int                chipId,
                                                     const std::string& registerName,
                                                     unsigned int       valueToWrite)
{
	std::cout << __COUT_HDR_FL__ << "Working on Register " << registerName
	          << " Address: " << getDACRegisterAddress(registerName)
	          << " Value: " << valueToWrite << std::endl;
	uint64_t command = 0;
	if(registerName != "SendData" && registerName != "RejectHits")
	{
		command = makeDACWriteHeader(chipId, registerName);
		BitManipulator::insertBits(
		    command, BitManipulator::reverseBits(valueToWrite, 0, 8), 13, 8);
	}
	else if(valueToWrite == 1)
		command = makeDACSetHeader(chipId, registerName);
	else
		command = makeDACResetHeader(chipId, registerName);
	// BitManipulator::insertBits(command,0x0,13,8);It doesn't matter the value you put in
	// if is set or reset!

	return command;
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeDACReadCommand(int                chipId,
                                                    const std::string& registerName)
{
	return makeDACHeader(chipId, registerName, read);
}

//==============================================================================
std::string PSI46DigROCDefinitions::makeMaskWriteCommand(int                chipId,
                                                         const std::string& registerName,
                                                         std::string        valueToWrite)
{
	std::string command = "0000000000000000000";

	/*Ryan's
	//Insert Chip ID
	//BitManipulator::insertBits(command,BitManipulator::reverseBits(chipId,0,5),0,5);
	BitManipulator::insertBits(command,chipId,0,5);
	//Insert Register Address
	//BitManipulator::insertBits(command,BitManipulator::reverseBits(getDACRegisterAddress(registerName),0,5),5,5);
	BitManipulator::insertBits(command,getDACRegisterAddress(registerName),5,5);
	//Insert instruction
	//BitManipulator::insertBits(command,writeReversed,10,3);
	BitManipulator::insertBits(command,write,10,3);

	UInt8 revField = (UInt8)0;
	for (unsigned int c=0; c<valueToWrite.length(); c++)
	{
	    if(registerName == "Inject")
	        revField = !(UInt8)valueToWrite.substr(c,1).data()[0];
	    else
	        revField = (UInt8)valueToWrite.substr(c,1).data()[0];
	    //revField = 0x40;
	    BitManipulator::insertBits(command,(uint64_t)revField,13+c,1);
	}

	std::cout << __COUT_HDR_FL__ << " std::string made: " << std::endl;
	for (int i=0; i<19; i++)
	{
	    std::cout << __COUT_HDR_FL__ << hex << (uint64_t)command.substr(i, 1).data()[0] <<
	"-" << std::endl;
	}
	std::cout << __COUT_HDR_FL__ << "\n" << std::endl;
*/
	return command;
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::makeMaskReadCommand(int                chipId,
                                                     const std::string& registerName)
{
	return 0;
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::setSendData(int chipId)
{
	return makeDACWriteCommand(chipId, "SendData", 1);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::resetSendData(int chipId)
{
	return makeDACWriteCommand(chipId, "SendData", 0);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::setRejectHits(int chipId)
{
	return makeDACWriteCommand(chipId, "RejectHits", 1);
}

//==============================================================================
uint64_t PSI46DigROCDefinitions::resetRejectHits(int chipId)
{
	return makeDACWriteCommand(chipId, "RejectHits", 0);
}
