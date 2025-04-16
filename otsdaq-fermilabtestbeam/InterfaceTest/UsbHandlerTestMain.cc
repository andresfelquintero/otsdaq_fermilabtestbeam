#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "otsdaq-fermilabtestbeam/CAMAC_XXUSB/UsbHandler.cpp"
#include "otsdaq-fermilabtestbeam/CAMAC_XXUSB/UsbHandler.h"

int main(int argc, char** argv)
{
	int  runNumber = 5;
	char path[256];
	sprintf(path, "%s/data_run_%d", std::__ENV__("USER_DATA"), runNumber);
	std::ofstream output = std::ofstream(path, std::ios::out | std::ios::trunc);
	if(!output.is_open())
	{
		std::cout << "Could not open file.";
		return -1;
	}
	std::time_t now =
	    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	char buf[100] = {0};
	std::strftime(buf, sizeof(buf), "%F %T", std::localtime(&now));
	std::string header = buf;
	header += "\n";
	long int len = header.length();
	output.write(header.c_str(), len);  // write header here
	UsbHandler* camac = new UsbHandler();
	std::cout << "Finding USB connection to CAMAC crate, configuring crate." << std::endl;
	camac->configureCrate();
	unsigned short int pdata[10][26700];
	int                ret = -2;
	ret                    = camac->getData(pdata, 1);
	printf("getData ret %d\n", ret);
	if(ret < 0)
	{
		camac->ccUsbFlush();
		xxusb_device_close(camac->crate);
		return -1;
	}
	int events = pdata[0][0] & 0xfff;
	for(int i = 2; events > 0; events--)
	{
		while(pdata[0][i] != 0xfeed)
		{
			output << pdata[0][i] << "\t";
			i++;
		}
		i += 2;
		output << "\n";
	}
	camac->ccUsbFlush();
	xxusb_device_close(camac->crate);
	return 0;
}
