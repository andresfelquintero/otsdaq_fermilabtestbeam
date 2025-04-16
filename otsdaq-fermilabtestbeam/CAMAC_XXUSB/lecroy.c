/*
 *  Application to talk to Lecroy 2249A Camac ADC
 *  Lecroy 2228A TDC and Jorway 85A Scaler via the Wiener,
 *  Plein und Baus CC-USB Camac interface
 *  (W.Badgett)
 */

#include "/home/nfs/ftbf_user/midas-daq/packages/libxx_usb/xxusb_3.22/include/libxxusb.h"
#include "fcntl.h"
#include "stdio.h"
#include "string.h"
#include "termios.h"
#include "time.h"
#include "unistd.h"
#include "usb.h"
//#include "libxxusb.h"
#include "errno.h"

#define LECROY_F_DATA 2
#define LECROY_F_LAM 8
#define LECROY_F_RESET 9
#define LECROY_F_START 25
#define LECROY_F_LAM_DISABLE 24
#define LECROY_F_LAM_ENABLE 26

#define LECROY2249A_CHANNELS 12
#define LECROY2228A_CHANNELS 8
#define LECROY2323A_CHANNELS 2

#define JORWAY85A_CHANNELS 4
#define JORWAY_F_CLEAR 9
#define JORWAY_F_DATA 0

#define CAMAC_DELAY 20000
#define CAMAC_MAX_TRIES 8
#define CAMAC_N_SLOTS 25

long int tdcData[CAMAC_N_SLOTS][LECROY2228A_CHANNELS];
long int adcData[CAMAC_N_SLOTS][LECROY2249A_CHANNELS];
long int scalData[CAMAC_N_SLOTS][JORWAY85A_CHANNELS];

int nADC[CAMAC_N_SLOTS];
int nTDC[CAMAC_N_SLOTS];
int nSCAL[CAMAC_N_SLOTS];
int nGATE[CAMAC_N_SLOTS];
int cADC  = 0;
int cTDC  = 0;
int cSCAL = 0;
int cGATE = 0;

struct termios terminalSettings, newSettings;

void resetData()
{
	int i, n;
	for(n = 0; n < CAMAC_N_SLOTS; n++)
	{
		for(i = 0; i < LECROY2228A_CHANNELS; i++)
		{
			tdcData[n][i] = -1;
		}
		for(i = 0; i < LECROY2249A_CHANNELS; i++)
		{
			adcData[n][i] = -1;
		}
		for(i = 0; i < JORWAY85A_CHANNELS; i++)
		{
			scalData[n][i] = -1;
		}
	}
}

void writeData(FILE* fptr)
{
	int i, j;
	for(i = 0; i < cADC; i++)
	{
		for(j = 0; j < LECROY2249A_CHANNELS; j++)
		{
			fprintf(fptr, "%ld\t", adcData[i][j]);
		}
	}  //*****************Changed by Alex 2/10/15 (suggested by Bill)

	for(i = 0; i < cTDC; i++)
	{
		for(j = 0; j < LECROY2228A_CHANNELS; j++)
		{
			fprintf(fptr, "%ld ", tdcData[i][j]);
		}
	}

	for(i = 0; i < cSCAL; i++)
	{
		for(j = 0; j < JORWAY85A_CHANNELS; j++)
		{
			fprintf(fptr, "%ld ", scalData[i][j]);
		}
	}
}

void doBadExit()
{
	printf("\n*** Failed CAMAC access, please try again ***\n\n");
	tcsetattr(STDIN_FILENO, TCSANOW, &terminalSettings);
	exit(ECOMM);
}

void checkResponse(int n, int f, int retcod, int q, int x)
{
	if((retcod <= 0) || (x != 1))
	{
		printf("*** Problem writing N=%d F=%d register retcod=%d Q=%d X=%d\n",
		       n,
		       f,
		       retcod,
		       q,
		       x);
		//    doBadExit();set
	}
}

void setGate(usb_dev_handle* crate, int n, int nChannels)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	int      i;
	long int data;

	if(n > 0)
	{
		for(i = 0; i < nChannels; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, LECROY_F_START, &data, &qResponse, &xResponse);
			checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);
		}
	}
}

void resetGate(usb_dev_handle* crate, int n, int nChannels)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	int      i;
	long int data;

	if(n > 0)
	{
		for(i = 0; i < nChannels; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, LECROY_F_RESET, &data, &qResponse, &xResponse);
			checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);
		}
	}
}

void readJorway(usb_dev_handle* crate, int n, int nChannels, long int* data)
{
	int retcod    = 0;
	int qResponse = 0;
	int xResponse = 0;
	int i;

	if(n > 0)
	{
		for(i = 0; i < nChannels; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, JORWAY_F_DATA, &data[i], &qResponse, &xResponse);
			checkResponse(n, JORWAY_F_DATA, retcod, qResponse, xResponse);
		}
	}
}

int checkLecroyLAM(usb_dev_handle* crate, int n)
{
	int retcod    = 0;
	int qResponse = 0;
	int xResponse = 0;

	if(n > 0)
	{
		/* Check for LAM presence via Q */
		retcod = CAMAC_read(crate, n, 0, LECROY_F_LAM, 0, &qResponse, &xResponse);
		checkResponse(n, LECROY_F_LAM, retcod, qResponse, xResponse);
	}
	else
	{
		qResponse = 1;
	}
	return (qResponse);
}

int readLecroy(usb_dev_handle* crate, int n, int nChannels, long int* data)
{
	int retcod    = 0;
	int qResponse = 0;
	int xResponse = 0;
	int i;

	if(n > 0)
	{
		for(i = 0; i < nChannels; i++)
		{
			retcod =
			    CAMAC_read(crate, n, i, LECROY_F_DATA, &data[i], &qResponse, &xResponse);
			checkResponse(n, LECROY_F_DATA, retcod, qResponse, xResponse);
		}
	}

	return (qResponse);
}

void configureJorway(usb_dev_handle* crate, int n)
{
	int      i, q, x, retcod;
	long int data;

	/* Clear scaler counts */

	for(i = 0; i < JORWAY85A_CHANNELS; i++)
	{
		retcod = CAMAC_read(crate, n, i, JORWAY_F_CLEAR, &data, &q, &x);
		checkResponse(n, JORWAY_F_CLEAR, retcod, q, x);
	}
}

void configureLecroy(usb_dev_handle* crate, int n)
{
	int      retcod    = 0;
	int      qResponse = 0;
	int      xResponse = 0;
	long int data;

	/* These are control operations, not really reads */
	retcod = CAMAC_read(crate, n, 0, LECROY_F_RESET, &data, &qResponse, &xResponse);
	checkResponse(n, LECROY_F_RESET, retcod, qResponse, xResponse);

	retcod = CAMAC_read(crate, n, 0, LECROY_F_LAM_ENABLE, &data, &qResponse, &xResponse);
	checkResponse(n, LECROY_F_LAM_ENABLE, retcod, qResponse, xResponse);
}

int main(int argc, char** argv)
{
	int                nDevices = 0, retcod;
	xxusb_device_type  xxdev;
	struct usb_device* ptr;
	usb_dev_handle*    crate;
	struct timespec    delay;
	struct timespec    remainder;
	char               hostName[256];
	int                qADC[CAMAC_N_SLOTS];
	int                qTDC[CAMAC_N_SLOTS];
	int                qADCsum;
	int                qTDCsum;
	char               configFile[256];
	FILE*              fptr;
	FILE*              cptr;
	char               fileName[256];
	char*              line;
	size_t             lineSize;
	char               key;
	int                trigger = 0;
	int                i;

	gethostname(hostName, sizeof(hostName) - 1);

	if(argc < 2)
	{
		printf("Usage:\n");
		printf("  lecroy <FileName Tag> <lecroy.config file>\n");
		printf("\nFileName Tag sets beginning of file name, required\n");
		printf("The lecroy.config file contains a list of cards:\n");
		printf("<Slot Number> <Card Type>\n");
		printf("Where card type can be ADC, TDC, SCAL or GATE\n");
		exit(0);
	}

	if(argc >= 3)
	{
		strcpy(configFile, argv[2]);
	}
	else
	{
		strcpy(configFile, "lecroy.config");
	}

	cptr = fopen(configFile, "r");
	if(cptr == NULL)
	{
		printf("*** Cannot open file <%s>\n", configFile);
		exit(-1);
	}

	printf("Reading configuration file <%s>\n", configFile);
	line     = (char*)malloc(256);
	lineSize = (size_t)256;
	while(getline(&line, &lineSize, cptr) > 0)
	{
		int  slot = 0;
		char type[80];

		sscanf(line, "%d %s", &slot, type);
		if((slot >= 1) && (slot <= CAMAC_N_SLOTS))
		{
			if(strcmp(type, "ADC") == 0)
			{
				nADC[cADC] = slot;
				cADC++;
				printf(" * ADC  %d slot %2d\n", cADC, slot);
			}
			else if(strcmp(type, "TDC") == 0)
			{
				nTDC[cTDC] = slot;
				cTDC++;
				printf(" * TDC  %d slot %2d\n", cTDC, slot);
			}
			else if(strcmp(type, "SCAL") == 0)
			{
				nSCAL[cSCAL] = slot;
				cSCAL++;
				printf(" * SCAL %d slot %2d\n", cSCAL, slot);
			}
			else if(strcmp(type, "GATE") == 0)
			{
				nGATE[cGATE] = slot;
				cGATE++;
				printf(" * GATE %d slot %2d\n", cGATE, slot);
			}
		}
	}
	fclose(cptr);

	delay.tv_sec  = 0;
	delay.tv_nsec = CAMAC_DELAY; /* 100 milliseconds */

	xxdev.SerialString[0] = '\0';
	nDevices              = xxusb_devices_find(&xxdev);
	printf("Found nDevices %d [%s] \n", nDevices, xxdev.SerialString);

	if(nDevices <= 0)
	{
		printf("No CC-USB devices found, exiting\n");
		exit(-1);
	}
	else if(nDevices > 1)
	{
		printf("Multiple CC-USB devices found, using first\n");
	}
	ptr = xxdev.usbdev;

	char stub[32];
	sprintf(fileName, "%s", argv[1]);
	for(i = 0; i < cADC; i++)
	{
		sprintf(stub, "_a%d", nADC[i]);
		strcat(fileName, stub);
	}
	for(i = 0; i < cTDC; i++)
	{
		sprintf(stub, "_t%d", nTDC[i]);
		strcat(fileName, stub);
	}
	for(i = 0; i < cSCAL; i++)
	{
		sprintf(stub, "_s%d", nSCAL[i]);
		strcat(fileName, stub);
	}
	strcat(fileName, ".dat");

	fptr = fopen(fileName, "w");
	if(fptr != NULL)
	{
		printf("Opened file %s for data taking\n", fileName);
	}
	else
	{
		printf("Filed to open file %s (%s)\n", fileName, strerror(errno));
		exit(errno);
	}

	tcgetattr(STDIN_FILENO, &terminalSettings);
	memcpy(&newSettings, &terminalSettings, sizeof(struct termios));

	newSettings.c_iflag &= ~(tcflag_t)IGNBRK;
	newSettings.c_iflag |= BRKINT;
	newSettings.c_lflag |= ISIG;
	newSettings.c_lflag &= ~(tcflag_t)ECHO;
	newSettings.c_lflag &= ~(tcflag_t)ICANON;
	newSettings.c_cc[VINTR] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	crate = xxusb_device_open(ptr);
	if(crate != NULL)
	{
		int tries = 0;
		printf("Opened device /dev/bus/usb/%s/%s\n", ptr->bus->dirname, ptr->filename);

		/* Avoid Z's.   Maybe C's, too? */
		retcod = -1;
		while((retcod < 0) && (tries < CAMAC_MAX_TRIES))
		{
			retcod = CAMAC_C(crate);
			tries++;
			if(retcod < 0)
			{
				printf("Failed to issue CAMAC C [%d] (%d tries)\n", retcod, tries);
				/* doBadExit();*/
			}
			else
			{
				printf("Issued CAMAC C [%d] (%d)\n", retcod, tries);
			}
			nanosleep(&delay, &remainder);
		}

		if(retcod < 0)
		{
			printf("Failed to issue CAMAC C [%d] (%d tries), giving up\n", retcod, tries);
			doBadExit();
		}

		retcod = CAMAC_I(crate, 0);
		if(retcod < 0)
		{
			printf("Failed to issue CAMAC I to 0 [%d]\n", retcod);
			doBadExit();
		}
		else
		{
			printf("Issued CAMAC I to 0 [%d]\n", retcod);
		}

		/* Configure Lecry 2249A ADC for LAMs */

		for(i = 0; i < cADC; i++)
		{
			configureLecroy(crate, nADC[i]);
		}
		for(i = 0; i < cTDC; i++)
		{
			configureLecroy(crate, nTDC[i]);
		}
		for(i = 0; i < cSCAL; i++)
		{
			configureJorway(crate, nSCAL[i]);
		}

		resetData();

		/* Initial clear of the gate generator(s) */
		for(i = 0; i < cGATE; i++)
		{
			setGate(crate, nGATE[i], LECROY2323A_CHANNELS);
		}

		nanosleep(&delay, &remainder);

		for(i = 0; i < cGATE; i++)
		{
			resetGate(crate, nGATE[i], LECROY2323A_CHANNELS);
		}

		while(key != 'q' && key != 'Q')
		{
			nanosleep(&delay, &remainder);
			qADCsum = 0;
			qTDCsum = 0;
			for(i = 0; i < cADC; i++)
			{
				qADC[i] = checkLecroyLAM(crate, nADC[i]);
				qADCsum += qADC[i];
			}
			for(i = 0; i < cTDC; i++)
			{
				qTDC[i] = checkLecroyLAM(crate, nTDC[i]);
				qTDCsum += qTDC[i];
			}

			if((qADCsum == cADC) && (qTDCsum == cTDC))
			{
				for(i = 0; i < cADC; i++)
				{
					readLecroy(crate, nADC[i], LECROY2249A_CHANNELS, adcData[i]);
				}

				for(i = 0; i < cTDC; i++)
				{
					readLecroy(crate, nTDC[i], LECROY2228A_CHANNELS, tdcData[i]);
				}

				trigger++;
				printf("\r%9d", trigger);
				fflush(stdout);
				// fprintf(fptr,"%d ", trigger);//*******************************Changed
				// by Alex 2/10/15 (suggested by Bill)
				for(i = 0; i < cSCAL; i++)
				{
					readJorway(crate, nSCAL[i], JORWAY85A_CHANNELS, scalData[i]);
				}

				writeData(fptr);
				fprintf(fptr, "\n");
				resetData();

				/* Clear the gate generator(s) so we can take more triggers */
				for(i = 0; i < cGATE; i++)
				{
					resetGate(crate, nGATE[i], LECROY2323A_CHANNELS);
				}
			}
			key = 0;
			read(STDIN_FILENO, &key, 1);
			if(key == 'p' || key == 'P')
			{
				printf("\nSCAs %d: ", cSCAL);
				printf("\nADCs %d: ", cADC);
				for(i = 0; i < cADC; i++)
				{
					printf(" %d=%d", nADC[i], qADC[i]);
				}
				printf("\nTDCs %d: ", cTDC);
				for(i = 0; i < cTDC; i++)
				{
					printf(" %d=%d", nTDC[i], qTDC[i]);
				}
				printf("\n");
			}
		}
		retcod = xxusb_device_close(crate);
	}
	else
	{
		printf("Failed to open crate device\n");
		doBadExit();
	}
	printf("\n");
	fclose(fptr);
	tcsetattr(STDIN_FILENO, TCSANOW, &terminalSettings);
	exit(0);
}
