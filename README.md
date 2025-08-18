# Otsdaq_FermilabTestbeam
DAQ for the instrumentation at FTBF

# Otsdaq Fermilab Testbeam

## OTSDAQ Core Installation
This code is an extension of the otsdaq project: https://github.com/art-daq/otsdaq/wiki project and requires it to function. 
The following instruction must be executed once and they are needed to install OTSDAQ.
Once the OTSDAQ core dependencies are set up, the CMS Tracker interface code can be included.  

# ALMA 9.5
Most of the packages needed to run OTSDAQ are hosted in the cern repo.
It is recommended to install the ALMA 9 CERN version.
To install the CERN ALMA 9 version outside CERN you should follow these instructions.

1. Download a generic Alma9 ISO, e.g. the DVD ISO linked from https://almalinux.org/get-almalinux/#ISO_Images.
2. Boot from that ISO
3. Add http://linuxsoft.cern.ch/cern/alma/9.5/CERN/x86_64/ as additional repository
4. Select the option to install the "CERN Workstation"

If you have another ALMA 9 version you need to install the cern repositories.
https://linux.web.cern.ch/updates/alma9/

# OTSDAQ INSTALLATION ON ALMA 9
<!--
### The manual installation instructions are collected into the following [Install.sh](https://gitlab.cern.ch/otsdaq/otsdaq_cmstracker/-/blob/develop/SetupFiles/Install.sh?ref_type=heads) script that you can just download and run!
### WARNING: The script will create the 'otsdaq' directory from the location where it is run. 
### You need sudo powers and need to install [protobuf](https://gitlab.cern.ch/cms_tk_ph2/MessageUtils/-/blob/master/README.md) yourself!
-->
## Manual installation
<!--
### If you didn't run the Install.sh script  then you can copy and paste in a terminal the following instructions
-->
### Install OTSDAQ system dependencies (need to be root or have sudo powers):

<pre>
sudo dnf install -y libdb-devel
sudo dnf install -y gdbm-devel
sudo dnf install -y libfontenc-devel
sudo dnf install -y libpciaccess libpciaccess-devel
sudo dnf install -y libtirpc-devel
sudo dnf install -y libXdmcp-devel libXxf86vm-devel Lmod mesa-libGLU mesa-libGLU-devel mesa-libGL-devel
sudo dnf install -y meson nasm ninja-build patchelf 
sudo dnf install -y python python3 python-devel python3-devel
sudo dnf install -y texinfo texlive mesa-libGL-devel
sudo dnf install -y xorg-x11-util-macros xorg-x11-server-devel xorg-x11-xtrans-devel
sudo dnf install -y libxcb libxcb-devel 
sudo dnf install -y xcb-util 
sudo dnf install -y xcb-util-wm xcb-util-wm-devel 
sudo dnf install -y xcb-util-keysyms xcb-util-keysyms-devel 
sudo dnf install -y xcb-util-image xcb-util-image-devel
sudo dnf install -y xcb-util-renderutil xcb-util-renderutil-devel
sudo dnf install -y libxkbcommon libxkbcommon-devel libxkbcommon-x11 libxkbcommon-x11-devel  
sudo dnf install -y libX11-xcb
sudo dnf install -y gcc-gfortran
sudo dnf install -y freetype freetype-devel
sudo dnf install -y binutils-devel
sudo dnf install -y patch
sudo dnf install -y gettext-devel
</pre>

## CAEN HV Wrapper libraries are needed to control the CAEN power supply (need to be root or have sudo powers).
Download the tar file here after logging in:
https://www.caen.it/?downloadfile=7011

<pre>
tar zxvf CAENHVWrapper-6.3.tgz 
cd CAENHVWrapper-6.3/
./install.sh 
</pre>
 

### The following instructions will install OTSDAQ and fermilabtestbeam package. They can be copied in a shell script and then run from there.

- This installation will create 2 directories:

    1. otsdaq/spack (where all the packages are installed)

    2. otsdaq/user (where we keep the data, configuration database and user data for OTSDAQ)

<pre>
###################################################
#FIRST: Create and go to the directory where you want to install OTSDAQ.
#As an example, here we create the otsdaq directory and we will cd into it
###################################################
mkdir otsdaq && cd otsdaq
export OTSDAQ_HOME=$PWD #This will be the directory where YOU want to install OTSDAQ
export CACTUSROOT=/opt/cactus

###################################################
#Installing Spack Fermilab stuff
###################################################
cd ${OTSDAQ_HOME}
mkdir -p spack/repos
cd spack
git clone https://github.com/FNALssi/spack.git -b fnal-develop
cd spack
git reset --hard 5d8ac7656ad4b8c2850e76d4751b9e0333d80d6b
cd ..
echo 'export SPACK_DISABLE_LOCAL_CONFIG=true 
source spack/share/spack/setup-env.sh' > setup-env.sh
source setup-env.sh
git clone https://github.com/fnalssi/fermi-spack-tools.git
cd fermi-spack-tools
git reset --hard 06627a1b963fefcfc922a4789696f1e5adf27511
cd ..
./fermi-spack-tools/bin/make_packages_yaml spack

###################################################
#Create and activate ots environment
###################################################
cd ..
spack compiler find
spack env create ots
spack env activate ots

###################################################
#Installing art suite
###################################################
cd ${OTSDAQ_HOME}
cd spack/repos
#git clone https://github.com/FNALssi/fnal_art.git && spack repo add fnal_art
git clone -b eflumerf/DontUseMasterCMake https://github.com/eflumerf/fnal_art.git && spack repo add fnal_art
#sed -i 's/libxml2@2.9.12/libxml2@2.9.13/g' ${OTSDAQ_HOME}/spack/repos/fnal_art/packages/art-suite/package.py
#git clone https://github.com/uplegger/fnal_art.git && spack repo add fnal_art
cd fnal_art
git reset --hard a150e04757263b39dd7b01140c0e7b4a20ac92e1

cd ${OTSDAQ_HOME}
spack add art-suite@s126
spack concretize -f
spack install -j`nproc`

###################################################
#Installing otsdaq suite
###################################################
cd ${OTSDAQ_HOME}
cd spack/repos

git clone https://github.com/art-daq/artdaq-spack.git && spack repo add artdaq-spack
cd artdaq-spack
git reset --hard e7f4a8998b0497ee6831a0ddb1e7ae0a487031bb

cd ${OTSDAQ_HOME}
spack add otsdaq-suite@v2_08_00 artdaq=31207 s=126
spack concretize -f
spack install -j`nproc`
spack install -j1

###################################################
#Installing Tracker and Burninbox packages
###################################################
cd ${OTSDAQ_HOME}
cd spack/repos
git clone https://gitlab.cern.ch/otsdaq/spack/otsdaq-fermilabtestbeam-spack && spack repo add otsdaq-fermilabtestbeam-spack

If this one does not work try the following line

git clone https://github.com/andresfelquintero/otsdaq-fermilabtestbeam-spack && spack repo add otsdaq-fermilabtestbeam-spack 

#After either of the lines work then run the following one

cd ..
spack add otsdaq-cms-fermilabtestbeam
spack concretize -f
spack install -j`nproc`
</pre>

### Get the SetupFile that needs to be sourced every time a new terminal is opened
<pre>
cd ${OTSDAQ_HOME}
source setup_ots.sh
</pre>


##  READ THIS FIRST BEFORE RUNNING OTSDAQ
**The Setup file you just copied (BurninBoxSetup.sh or TrackerSetup.sh) is a generic setup file, which is very similar to the Setup file example in the next paragraph.**


**There are 5 important variables that needs to be modified in order for OTSDAQ to work in YOUR environment**
1) USER_DATA
2) ARTDAQ_DATABASE_URI
3) OTSDAQ_DATA
4) BURNINBOX_CONFIGURATION_FILE
5) MODULE_NAMES_FILE

Variables meaning:
1) USER_DATA  -> points to a directory where Logs, RunNumber and other infos are stored. Each center will have its own USER_DATA directory.
2) ARTDAQ_DATABASE_URI -> has the database with the history of all configurations
3) OTSDAQ_DATA -> is the directory where **ALL DATA WILL BE SAVED**
4) BURNINBOX_CONFIGURATION_FILE -> if you use the BurninBox, you MUST configure this file according to the way YOU wired your box
5) MODULE_NAMES_FILE -> is the file where the module names are stored when a burnin cycle starts

**My reccomendation is to copy the default configurations to your custom configurations and then modify them accordingly**

**EXAMPLE**
I like to use my institution name to store those quantities so in the following example I will use Fermilab. For the other centers try to use any of the following please:
Brussels
DESY
Louvain
NCP
Niser
Pisa
Princeton
Rutgers

##  Running OTSDAQ the VERY FIRST TIME ONLY!!
From a terminal window
<pre>
cd ${OTSDAQ_HOME}
source setup_ots.sh #Needs to be run only once when a new terminal is opened
ots -w
</pre>

The **ots -w** command transforms some environmental variables into the link they correspond to. This command must be executed ONLY the very first time after a clean installation of OTSDAQ. 

##  Running OTSDAQ
From a terminal window
<pre>
cd ${OTSDAQ_HOME}
source setup_ots.sh #Needs to be run only once when a new terminal is opened
ots
</pre>

Copy and paste in Firefox or Chrome the link that appears on the terminal when OTSDAQ has started.


##  Setup file example
```sh

export CONFIGURATION_NAME=Default
#export PROJECT_HOME=$(echo $PWD | sed -e 's|/user||')
export PROJECT_HOME=$PWD
export USER_DATA_HOME=${PROJECT_HOME}/otsdaq-cms-burninbox-data/${CONFIGURATION_NAME}
export SPACK_HOME=${PROJECT_HOME}/spack
export OTS_MAIN_PORT=2015

export SPACK_DISABLE_LOCAL_CONFIG=true 
source ${SPACK_HOME}/spack/share/spack/setup-env.sh

spack env activate ots

#return
export MRB_SOURCE=$PROJECT_HOME

##### DATA DIRECTORY CONFIGURATION ##########
#OTSDAQ_DATA is the dirctory where all histograms are stored!!
export OTSDAQ_DATA="${PROJECT_HOME}/OtsdaqData/BurninBoxOtsdaqData"

##### OTSDAQ MAIN ENVIRONMENT VARIABLES CONFIGURATION ##########
#USER_DATA is the directory where RunNumber, Logs and other user informations are stored
export USER_DATA="${USER_DATA_HOME}/UserData"
#ARTDAQ_DATABASE_URI is the variable pointing to the USER database
export ARTDAQ_DATABASE_URI="filesystemdb://${USER_DATA_HOME}/Database"

##### BURNIN BOX CONFIGURATION ##########
#Each center has its own specific configuration for the sensors on the controller - NCP
export BURNINBOX_CONFIGURATION_FILE="${USER_DATA_HOME}/HardwareConfiguration/BurninBoxConfiguration_${CONFIGURATION_NAME}.xml"
export MODULE_NAMES_FILE="${USER_DATA_HOME}/tmp/ModuleNames.cfg"

##### OUTERTRACKER CONFIGURATION ##########
#export OTSDAQ_CMSTRACKER_DIR=${SPACK_HOME}/otsdaq-cmstracker
export PH2ACF_BASE_DIR=${OTSDAQ_CMSTRACKER_DIR}/otsdaq-cmstracker/Ph2_ACF
export ACFSUPERVISOR_ROOT=${OTSDAQ_CMSTRACKER_DIR}/otsdaq-cmstracker/ACFSupervisor

ln -fs ${OTSDAQ_CMSBURNINBOX_DIR}/UserWebGUI ${OTSDAQ_UTILITIES_DIR}/WebGUI/CMSBurninBoxWebPath

echo -e "setup [275]  \t Now your user data path is USER_DATA \t\t = ${USER_DATA}"
echo -e "setup [275]  \t Now your database path is ARTDAQ_DATABASE_URI \t = ${ARTDAQ_DATABASE_URI}"
echo -e "setup [275]  \t Now your output data path is OTSDAQ_DATA \t = ${OTSDAQ_DATA}"
echo
echo
echo -e "setup [275]  \t Now use 'ots --wiz' to configure otsdaq"
echo -e "setup [275]  \t Then use 'ots' to start otsdaq"
echo -e "setup [275]  \t Or use 'ots --help' for more options"
echo
echo -e "setup [275]  \t Use 'kx' to kill otsdaq processes"
echo

alias kx='ots -k'
```
