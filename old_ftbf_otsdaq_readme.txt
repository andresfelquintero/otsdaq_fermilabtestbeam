###############################
So, you want to run the FTBF daq using otsdaq? Here are the three steps!

1. Log into ftbf-daq-08.fnal.gov as ftbf_user (once logged in do this: ksu ftbf_user).


2. Navigate to /home/nfs/ftbf_user/Desktop (e.g.: cd ~/Desktop).


3a. Setup and run otsdaq:

source setup_otsdaq.sh run

       - The above line sets up otsdaq, runs it, and opens the web GUI in firefox.
       - When firefox opens, you might have to select a user; select any user you like, e.g. DEFAULT_USER.
       - Note!!! For fermilab web security to be happy, we access the web GUI through https, so the first time you must add an exception in your browser for the security certificate. 
       (In Firefox, click "Advanced" --> "Add Exception..." --> "Confirm Security Exception"; in Chrome, click "Advanced" --> "Proceed to ftbf-daq-08.fnal.gov (unsafe)")
       - The web GUI will be faster/smoother if you use firefox or chrome on your local computer; https://ftbf-daq-08.fnal.gov is accessible anywhere on-site at Fermilab.
       (You may need a CILogin certificate: follow instructions here https://cilogon.org/)


3b. Check the name of the folder (usually we change it for each experimental run):

echo $OTSDAQ_RUN_NAME  

     - you can check existing run folders here: ll $OTSDAQ_FTBF_RUNS_PATH
     - all data for the run will end up here: ll $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME


3c. If you want to change the name of your run to an existing run, do this and then go back to step 3a.:

export OTSDAQ_RUN_NAME=YEAR_MONTH_MONTHNAME_TNUMBER

       - e.g 2018_09_September_T992





###############################
So, you want to do start a new FTBF experiment run with otsdaq?

1. Log into ftbf-daq-08.fnal.gov as ftbf_user (once logged in do this: ksu ftbf_user).
2. Navigate to /home/nfs/ftbf_user/Desktop (e.g.: cd ~/Desktop).
3. Run this script:

source new_otsdaq_experiment.sh YEAR_MONTH_MONTHNAME_TNUMBER

The above script will create a new folder with the default otsdaq settings (it copies template versions of User Data and the configuration database from the fermilab testbeam repository).



###############################
So, you want to do some slightly advanced otsdaq things?


If you omit the run in "source setup_otsdaq.sh run," it will just do the setup steps and you could start up otsdaq manually:

   StartOTS.sh 	    #for normal mode
   StartOTS.sh -w   #for wiz mode
   StartOTs.sh -k   #to kill all things ots

There are environment variables for three important folders:

 - $USER_DATA: all user specific data goes here
 - $ARTDAQ_DATABASE_URI: all configuration data goes here
 - $OTSDAQ_DATA: all output data goes here

Log files should be written to /data-08/otsdaq_develop/daqlogs. 

The DAQ desktop web GUI will be accessible at https://ftbf-daq-08.fnal.gov anywhere on-site.

To configure otsdaq, select the configure icon on the desktop and open the tree
view for the active configuration.

Navigate to FECherenkovInterface and expand the tree. You should see only one
member, Cherenkov. Expand this as well.

Here are the configuration options:

ADCList - expects a list of integers from 1 to 24, indicating what slots in the
	CAMAC crate have Lecroy 2249a ADCs. Enter 0 if there are no ADCs.
TDCList - same as above but for Lecroy 2228a TDCs
ScalerList - same as above but for Jorway 85a scalers.
GateList - same as above but for Lecroy 2323a gate generators.
LoopTime - when the front end is running, it calls getData() in a loop. getData
	sets the Wiener cc-usb to daq mode, waits LoopTime seconds, and turns
	off daq mode to read the output FIFO.
DataFilenamePrefix - data files output from otsdaq are located in $USER_DATA
	and take the form DataFilenamePrefix### where ### is the current run
	number.
DataFileHeader - This header appears at teh top of data files, under the date
	and time of the run start and above the list of CAMAC modules in use.

After you have set the configuration options as you wish, scroll to the top of
the tree and save your changes as a new configuration. By default, this
configuration will be activated.

To take data, open the State Machine icon on the otsdaq desktop. Select
initialize. Ensure that the correct configuration is selected and select
configure. At this point the front end code prepares the modules in the CAMAC
crate and the CAMAC controller to take data, including preparing the stack.
Finally select run. The front end code creates a new output file for this 
run number and begins taking data in a loop. You can pause the run and resume
it later or you can stop the run to shutdown the program and start a new one.


If everything works properly, these will be the only important instructions.

Things I hope no one ever has to do:

To change the format of the configuration table, you need to start otsdaq in
wizard mode with 'StartOts.sh -w'. otsdaq also has a verbose option -v for
debugging.

If you're seeing a lot of error code -110, try completely restarting otsdaq.
If that doesn't work, it is likely that the Wiener cc-usb has been left in a
bad state. Sometimes it needs to power cycled to recover.

The front end source code is located in the source directory of otsdaq, which
is currently /data/otsdaq_develop/srcs/ . The front end code can be found in 
srcs/otsdaq_fermilabtestbeam/otsdaq-fermilabtestbeam/ in three seperate places.
These are FEInterfaces, where the FECherenkovInterface that otsdaq looks for is
implemented, CAMAC_XXUSB which holds the code to communicate with the cc-usb
used by the interface, and xxusb_3.3, which contains the library from Wiener to
use with the cc-usb.

To compile the code, you must have run the otsdaq setup script. The command is
'mrb b', and the command to clear previous builds is 'mrb z'. It works best if
you run these as otsdaq_develop while ssh'ed into ftbf-daq-07. Alternatively,
compile it here and be prepared to bug someone with sudo access to update or 
install some libraries.

The standalone C program to operate the cc-usb is in the directory
/home/nfs/ftbf_user/experiments/alex/ALEX
The latest version of the source is cherenkov_clean.c, the script to compile
the program is compile_lecroy.sh, and the executable is lecroy_exec. Many
things are hard coded in this program, and the latest version supports only
ADCs. Older versions, including the original program found in many places
on the machine (called lecroy.c) contain code to use other modules, but
do not make use of the cc-usb's stack operation like cherenkov_clean.c,
lecroy3.c, and otsdaq. This program is mostly useful for troubleshooting the
cc-usb.
