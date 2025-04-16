/*
 * UsbHandler.h
 *
 *  Created on: Jun 15, 2018
 *      Author: otsdaq_develop
 */

#ifndef OTSDAQ_FERMILABTESTBEAM_OTSDAQ_FERMILABTESTBEAM_CAMAC_XXUSB_USBHANDLER_H_
#define OTSDAQ_FERMILABTESTBEAM_OTSDAQ_FERMILABTESTBEAM_CAMAC_XXUSB_USBHANDLER_H_

#include <stdio.h>
#include <vector>
#include "otsdaq-fermilabtestbeam/xxusb_3.3/include/libxxusb.h"

typedef short unsigned int     sui;
typedef std::array<sui, 26700> ccusb_buf;
typedef std::vector<ccusb_buf> ccusb_buf_vec;

class UsbHandler
{
  public:
	UsbHandler();
	virtual ~UsbHandler();
	bool          checkResponse(int, int, int, int, int);
	void          setGate(int);
	void          resetGate(int);
	void          configureJorway();
	void          configureLecroy(int n);
	std::string   setConfig(std::string ADC,
	                        std::string TDC,
	                        std::string Scal,
	                        std::string Gate);
	int           configureCrate();
	ccusb_buf_vec getData();
	int           ccUsbFlush();

  private:
	const static int LECROY_F_DATA        = 2;
	const static int LECROY_F_LAM         = 8;
	const static int LECROY_F_RESET       = 9;
	const static int LECROY_F_START       = 25;
	const static int LECROY_F_LAM_DISABLE = 24;
	const static int LECROY_F_LAM_ENABLE  = 26;

	const static int LECROY2249A_CHANNELS = 12;
	const static int LECROY2228A_CHANNELS = 8;
	const static int LECROY2323A_CHANNELS = 2;

	const static int JORWAY85A_CHANNELS = 4;
	const static int JORWAY_F_CLEAR     = 9;
	const static int JORWAY_F_DATA      = 0;

	const static int CAMAC_N_SLOTS = 25;

	int nADC[CAMAC_N_SLOTS];
	int nTDC[CAMAC_N_SLOTS];
	int nSCAL[CAMAC_N_SLOTS];
	int nGATE[CAMAC_N_SLOTS];
	int cADC  = 0;
	int cTDC  = 0;
	int cSCAL = 0;
	int cGATE = 0;

	int                nDevices = 0;
	xxusb_device_type  xxdev;
	struct usb_device* ptr;

	usb_dev_handle* crate;
};

#endif /* OTSDAQ_FERMILABTESTBEAM_OTSDAQ_FERMILABTESTBEAM_CAMAC_XXUSB_USBHANDLER_H_ */
