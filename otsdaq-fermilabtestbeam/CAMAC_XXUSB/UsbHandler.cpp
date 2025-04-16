/*
 * UsbHandler.cpp
 *
 *  Created on: Jun 15, 2018
 *      Author: otsdaq_develop
 */

#include "UsbHandler.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <usb.h>
#include <array>
#include <sstream>
#include <string>
#include "otsdaq/Macros/CoutMacros.h"
#include "otsdaq-fermilabtestbeam/xxusb_3.3/include/libxxusb.h"

UsbHandler::UsbHandler() : crate(nullptr) {}

UsbHandler::~UsbHandler()
{
	// If the CCUSB has been initialized, try to leave it in a good state.
	if(crate != nullptr)
	{
		xxusb_register_write(crate, 1, 0x0);
		ccUsbFlush();
		xxusb_device_close(crate);
		crate = nullptr;
	}
}

bool UsbHandler::checkResponse(int n, int f, int retcod, int q, int x)
{
	if((retcod <= 0) || (x != 1))
	{
		char stub[256];
		sprintf(stub,
		        "*** Problem writing N=%d F=%d register retcod=%d Q=%d X=%d\n",
		        n,
		        f,
		        retcod,
		        q,
		        x);
		__MOUT_ERR__ << stub;
		return false;
	}
	return true;
}

void UsbHandler::setGate(int n)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	int      i;
	long int data;

	if(crate == nullptr)
		return;
	if(n > 0)
	{
		for(i = 0; i < LECROY2323A_CHANNELS; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, LECROY_F_START, &data, &qResponse, &xResponse);
			checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);
		}
	}
}

void UsbHandler::resetGate(int n)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	int      i;
	long int data;

	if(crate == nullptr)
		return;
	if(n > 0)
	{
		for(i = 0; i < LECROY2323A_CHANNELS; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, LECROY_F_RESET, &data, &qResponse, &xResponse);
			checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);
		}
	}
}

void UsbHandler::configureJorway()
{
	int      i, j, q, x, retcod;
	long int data;

	/* Clear scaler counts */

	if(crate == nullptr)
		return;
	for(j = 0; j < cSCAL; j++)
	{
		for(i = 0; i < JORWAY85A_CHANNELS; i++)
		{
			retcod = CAMAC_read(crate, nSCAL[j], i, JORWAY_F_CLEAR, &data, &q, &x);
			checkResponse(nSCAL[j], JORWAY_F_CLEAR, retcod, q, x);
		}
	}
}

void UsbHandler::configureLecroy(int n)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	long int data;

	/* These are control operations, not really reads */

	if(crate == nullptr)
		return;
	retcod = CAMAC_read(crate, n, 0, LECROY_F_RESET, &data, &qResponse, &xResponse);
	checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);

	retcod = CAMAC_read(crate, n, 0, LECROY_F_LAM_ENABLE, &data, &qResponse, &xResponse);
	checkResponse(n, LECROY_F_LAM_ENABLE, retcod, qResponse, xResponse);
}

std::string UsbHandler::setConfig(std::string ADC,
                                  std::string TDC,
                                  std::string Scal,
                                  std::string Gate)
{
	std::istringstream reader;
	std::ostringstream writer;
	int                slot;

	writer.str("");

	if(!ADC.empty())
	{
		reader.str(ADC);
		while(reader.good())
		{
			reader >> slot;
			if(!reader.fail() && slot > 0 && slot < CAMAC_N_SLOTS)
			{
				nADC[cADC] = slot;
				cADC++;
				writer << "ADC " << cADC << " in Slot " << slot << std::endl;
			}
			else
				__MOUT_ERR__ << "Error reading ADC list, invalid format: " << reader.str()
				             << std::endl;
		}
		reader.clear();
	}

	if(!TDC.empty())
	{
		reader.str(TDC);
		while(reader.good())
		{
			reader >> slot;
			if(!reader.fail() && slot > 0 && slot < CAMAC_N_SLOTS)
			{
				nTDC[cTDC] = slot;
				cTDC++;
				writer << "TDC " << cTDC << " in Slot " << slot << std::endl;
			}
			else
				__MOUT_ERR__ << "Error reading TDC list, invalid format: " << reader.str()
				             << std::endl;
		}
		reader.clear();
	}

	if(!Scal.empty())
	{
		reader.str(Scal);
		while(reader.good())
		{
			reader >> slot;
			if(!reader.fail() && slot > 0 && slot < CAMAC_N_SLOTS)
			{
				nSCAL[cSCAL] = slot;
				cSCAL++;
				writer << "Scaler " << cSCAL << " in Slot " << slot << std::endl;
			}
			else
				__MOUT_ERR__ << "Error reading Scaler list, invalid format: "
				             << reader.str() << std::endl;
		}
		reader.clear();
	}

	if(!Gate.empty())
	{
		reader.str(Gate);
		while(reader.good())
		{
			int slot;
			reader >> slot;
			if(!reader.fail() && slot > 0 && slot < CAMAC_N_SLOTS)
			{
				nGATE[cGATE] = slot;
				cGATE++;
				writer << "Gate " << cGATE << " in Slot " << slot << std::endl;
			}
			else
				__MOUT_ERR__ << "Error reading Gate list, invalid format: "
				             << reader.str() << std::endl;
		}
	}
	return writer.str();
}

/* Append a command to the stack. */
void StackApp(long int* stack, long int command)
{
	stack[0]++;
	stack[stack[0]] = command;
}

/* Append an NAF command to the stack. */
void StackNAF(long int* stack,
              int       N,
              int       A,
              int       F,
              int       A_range,
              bool      waitForLAM = false,
              bool      longMode   = false,
              bool      fastCAMAC  = false)
{
	long int command =
	    (1 << 15) + (N << 9) + (A << 5) +
	    F;  // NAF command with another word following for complex commands.
	if(longMode)
		command += (1 << 14);  // 24 bit read or write
	StackApp(stack, command);
	command = (1 << 15);  // Start the next word with the continuation bit for the number
	                      // of repetitions
	if(waitForLAM)
		command += (1 << 7);  // Wait for LAM
	if(fastCAMAC)
		command += (1 << 6);  // Repeat in repeat mode (RM)
	else
		command += (1 << 5);  // Iterate over A
	StackApp(stack, command);
	StackApp(stack, A_range);  // Number of repetitions
}

/* Append an NAF command to the stack. */
void StackNAF(long int* stack,
              int       N,
              int       A,
              int       F,
              bool      waitForLAM = false,
              bool      longMode   = false,
              bool      fastCAMAC  = false)
{
	long int command =
	    (0 << 15) + (N << 9) + (A << 5) +
	    F;  // NAF command with another word following for complex commands.
	if(longMode)
		command += (1 << 14);  // 24 bit read or write
	StackApp(stack, command);
}

int UsbHandler::configureCrate()
{
	xxdev.SerialString[0] = '\0';
	nDevices              = xxusb_devices_find(&xxdev);
	__MOUT__ << "Found nDevices " << nDevices << " [" << xxdev.SerialString << "] \n";

	if(nDevices <= 0)
	{
		__MOUT_ERR__ << "No CC-USB devices found, exiting\n";
		return -1;
	}

	ptr = xxdev.usbdev;

	crate = xxusb_device_open(ptr);
	if(!crate)
	{
		__MOUT_ERR__ << "Failed to open ccusb, exiting \n";
		return -1;
	}
	else
	{
		int retcod = xxusb_register_write(crate, 1, 0x0);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Initial DAQ off returned " << retcod << "\n";
			return retcod;
		}
		ccUsbFlush();

		__MOUT__ << "Opened device /dev/bus/usb/" << ptr->bus->dirname << "/"
		         << ptr->filename << "\n";

		retcod = -2;

		retcod = CAMAC_C(crate);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed to issue CAMAC C [" << retcod << "]\n";
			return retcod;
		}

		retcod = CAMAC_I(crate, 0);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed to issue CAMAC I [" << retcod << "]\n";
			return retcod;
		}

		long int stack[500] = {0};

		for(int i = 0; i < cADC; i++)
		{
			//			StackApp(stack, 0x0010); Add extra markers to output to help
			// formatting - turns out cc-usb isn't consistent enough to do this well
			//			StackApp(stack, 0xff00+i);
			StackNAF(stack, nADC[i], 0, LECROY_F_DATA, LECROY2249A_CHANNELS, true);
		}
		for(int i = 0; i < cTDC; i++)
		{
			//			StackApp(stack, 0x0010);
			//			StackApp(stack, 0xff20+i);
			StackNAF(stack, nTDC[i], 0, LECROY_F_DATA, LECROY2228A_CHANNELS, true);
		}
		for(int i = 0; i < cSCAL; i++)
		{
			//			StackApp(stack, 0x0010);
			//			StackApp(stack, 0xff40+i);
			StackNAF(stack,
			         nSCAL[i],
			         0,
			         JORWAY_F_DATA,
			         JORWAY85A_CHANNELS,
			         false,
			         true,
			         false);
		}
		//		StackApp(stack, 0x0010);
		//		StackApp(stack, 0xff60);
		for(int i = 0; i < cADC; i++)
			StackNAF(stack, nADC[i], 0, LECROY_F_RESET);
		for(int i = 0; i < cTDC; i++)
			StackNAF(stack, nTDC[i], 0, LECROY_F_RESET);
		for(int i = 0; i < cADC; i++)
			StackNAF(stack, nADC[i], 0, LECROY_F_LAM_ENABLE);
		for(int i = 0; i < cTDC; i++)
			StackNAF(stack, nTDC[i], 0, LECROY_F_LAM_ENABLE);
		for(int i = 0; i < cGATE; i++)
			StackNAF(stack, nGATE[i], 0, LECROY_F_RESET, LECROY2323A_CHANNELS);

		StackApp(stack, 0x0010);
		StackApp(stack, 0xffff);

		__MOUT__ << "Finished creating the stack." << std::endl;

		int q;
		int x;

		retcod = xxusb_stack_write(crate, 2, stack);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed to write stack to ccusb. [" << retcod << "]\n";
			return retcod;
		}

		long stackCheck[500];
		retcod = xxusb_stack_read(crate, 2, &stackCheck[0]);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed to read stack from ccusb. [" << retcod << "]\n";
			return retcod;
		}

		size_t ii = 0;
		while(stack[ii] != 0xffff)
		{
			if(stack[ii] != stackCheck[ii])
			{
				__MOUT_ERR__ << "Stack was not written correctly! At word " << ii
				             << ", expected " << stack[ii] << ", got " << stackCheck[ii];
				return -25;
			}
			++ii;
		}

		long int d16 = ((0x00 & 0xFF) << 8) | (0x00 & 0xFF);
		retcod       = CAMAC_write(crate, 25, 2, 16, d16, &q, &x);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed CAMAC write. [" << retcod << "]\n";
			return retcod;
		}
		ccUsbFlush();

		retcod = CAMAC_write_LAM_mask(
		    crate, 0x0);  // Set LAM mask, 24 bit word, bit position=N, 0x0 triggers on I1
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed CAMAC write. [" << retcod << "]\n";
			return retcod;
		}
		retcod = CAMAC_write(
		    crate,
		    25,
		    1,
		    16,
		    0,
		    &q,
		    &x);  // Set the size of the buffer for the output FIFO and format the header
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed CAMAC write. [" << retcod << "]\n";
			return retcod;
		}
		/* Configure Lecroy 2249A ADC for LAMs */

		for(int i = 0; i < cADC; i++)
		{
			configureLecroy(nADC[i]);
		}
		for(int i = 0; i < cTDC; i++)
		{
			configureLecroy(nTDC[i]);
		}
		configureJorway();

		/* Initial clear of the gate generator(s) */
		for(int i = 0; i < cGATE; i++)
		{
			setGate(nGATE[i]);
		}

		for(int i = 0; i < cGATE; i++)
		{
			resetGate(nGATE[i]);
		}
	}
	return 1;
}

ccusb_buf_vec UsbHandler::getData()
{
	ccusb_buf_vec cc_data_vec;
	if(crate == nullptr)
		return cc_data_vec;

	int retcod = xxusb_register_write(crate, 1, 0x0);
	if(retcod < 0)
	{
		__MOUT_ERR__ << "Failed to turn DAQ mode off. [" << retcod << "]\n";
	}
	int  numReads = 0;
	sui* pdata;
	do
	{
		ccusb_buf cc_data;
		pdata  = cc_data.data();
		retcod = xxusb_bulk_read(crate, pdata, 26700, 100);
		if(retcod < 0)
		{
			__MOUT_ERR__ << "Failed USB bulk read. [" << retcod << "]\n";
			break;
		}
		cc_data_vec.push_back(cc_data);
		// TLOG(TLVL_TRACE, __MF_SUBJECT__) << "Loop number " << numReads << " returned "
		// << retcod << " with header byte " << std::hex << pdata[0] << std::dec << ".\n";
		numReads++;
	} while(!(pdata[0] & 0x8000));

	retcod = xxusb_register_write(crate, 1, 0x1);
	if(retcod < 0)
	{
		__MOUT_ERR__ << "Failed to turn DAQ mode on. [" << retcod << "]\n";
	}

	return cc_data_vec;
}

int UsbHandler::ccUsbFlush()
{
	short IntArray[10000];  // for FIFOREAD
	int   ret = 1;
	int   k   = 0;
	if(crate == nullptr)
		return -1;
	while(ret > 0)
	{
		ret = xxusb_bulk_read(crate, IntArray, 8192, 100);
		if(ret > 0)
		{
			__MOUT__ << "Flushing loop " << k << " from CCUSB buffer. Returned " << ret
			         << " bytes." << std::endl;
			k++;
		}
	}
	return (IntArray[0] & 0xfff);
}
