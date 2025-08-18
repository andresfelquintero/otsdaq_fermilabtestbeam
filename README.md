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
## Automatic installation

This code is meant to work with ostdaq v_03_02 . Several changes were made compared to the first version that worked with version 02_08. 
https://github.com/art-daq/otsdaq-demo/blob/develop/tools/ots-quick-spack-start.sh was used to install otsdaq in develop mode. 
Given that Spack already upgraded to V1.0, this installation code will need to be reworked to include a new develop version. 
However there are no issues installing it and it will work properly with the setup file that is included, although it needs to be adjusted to your needs.

Proper installation of this package will require that you use otsdaq_fermilabtestbeam_spack repository. 
It is located at https://github.com/andresfelquintero/otsdaq-fermilabtestbeam-spack or https://gitlab.cern.ch/otsdaq/spack/otsdaq-fermilabtestbeam-spack

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
echo 'export SPACK_DISABLE_LOCAL_CONFIG=true 
source spack/share/spack/setup-env.sh' > setup-env.sh
source setup-env.sh
git clone https://github.com/fnalssi/fermi-spack-tools.git
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
git clone -b eflumerf/DontUseMasterCMake https://github.com/eflumerf/fnal_art.git && spack repo add fnal_art

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

cd ${OTSDAQ_HOME}
spack add otsdaq-suite
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
echo # This script is intended to be sourced.

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
otsdir=$SCRIPT_DIR

sh -c "[ `ps $$ | grep bash | wc -l` -gt 0 ] || { echo 'Please switch to the bash shell before running ots.'; exit; }" || exit
export SPACK_DISABLE_LOCAL_CONFIG=true
source /home/nfs/emdaq/andres_develop/spack/share/spack/setup-env.sh

spack load --first gcc@13.1.0
spack compiler find

spack load otsdaq-utilities

spack env activate ots-develop
if [ -d /home/nfs/emdaq/andres_develop/local/install ]; then
  export PATH=/home/nfs/emdaq/andres_develop/local/install/bin:$PATH
  export LD_LIBRARY_PATH=/home/nfs/emdaq/andres_develop/local/install/lib:$LD_LIBRARY_PATH
  export CET_PLUGIN_PATH=/home/nfs/emdaq/andres_develop/local/install/lib:$CET_PLUGIN_PATH
  export FHICL_FILE_PATH=/home/nfs/emdaq/andres_develop/local/install/fcl:

  export OTSDAQ_DIR=${OTSDAQ_DIR:-$SCRIPT_DIR/local/install} #only set if not set by spack, e.g. needed by UpdateOTS.sh
  export OTSDAQ_LIB=${OTSDAQ_LIB:-$SCRIPT_DIR/local/install/lib} #only set if not set by spack, e.g. needed by otsConfiguration_Wizard_CMake.xml, otsConfiguration_MacroMaker_CMake.xml
  export OTSDAQ_UTILITIES_LIB=${OTSDAQ_UTILITIES_LIB:-$SCRIPT_DIR/local/install/lib} #only set if not set by spack, needed by otsConfiguration_Wizard_CMake.xml, otsConfiguration_MacroMaker_CMake.xml
  export OTSDAQ_UTILITIES_DIR=$SCRIPT_DIR/srcs/otsdaq-utilities

  # in ots-develop mode, set WebPath because OTSDAQ_UTILITIES_DIR is not setup
  if [ -d $SCRIPT_DIR/srcs/otsdaq-utilities/WebGUI ]; then
      export OTSDAQ_WEB_PATH=$SCRIPT_DIR/srcs/otsdaq-utilities/WebGUI
  else
      export OTSDAQ_WEB_PATH=$OTSDAQ_UTILITIES_LIB/../WebGUI
  fi
  export OTS_FILE_PARSE_PATTERN="/srcs/" #will be used to parse filename (i.e. for TRACE)
fi

k5user=`klist|grep "Default principal"|cut -d: -f2|sed 's/@.*//;s/ //'`
export TRACE_FILE=/tmp/trace_buffer_$USER.$k5user

export OTS_MAIN_PORT=2015

export USER_DATA="/home/nfs/emdaq/andres_develop/otsdaq_fermilabtestbeam/emphatic/2022_01_January_emphatic_userdata"
export ARTDAQ_DATABASE_URI="filesystemdb:///home/nfs/emdaq/andres_develop/otsdaq_fermilabtestbeam/emphatic/2022_01_January_emphatic_database"
export OTSDAQ_DATA="/home/nfs/emdaq/andres_develop/otsdaq_fermilabtestbeam/DataFTBF"
export OTS_SOURCE=/home/nfs/emdaq/andres_develop/srcs

echo -e "setup_ots.sh:${LINENO} |  \t  Now your user data path is USER_DATA \t\t = ${USER_DATA}"
echo -e "setup_ots.sh:${LINENO} |  \t  Now your database path is ARTDAQ_DATABASE_URI \t = ${ARTDAQ_DATABASE_URI}"
echo -e "setup_ots.sh:${LINENO} |  \t  Now your output data path is OTSDAQ_DATA \t = ${OTSDAQ_DATA}"
echo

#make the number of build threads dependent on the number of cores on the machine:
export CETPKG_J=$((19 + 1))

alias  kx='ots -k'
# When using upstream spack-mpd
#alias  mb='date; start_time=$(date +%s); spack find | grep gcc; spack mpd build -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g | sed s/\\[padded-to-255-chars\\]//g | sed s/\\/tdaq-v......../\\/tdaq-v_\ \ \ /g; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=$((end_time - start_time)); fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"'
#alias  ml='date; start_time=$(date +%s); spack find | grep gcc; spack mpd build -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g | sed s/\\[padded-to-255-chars\\]//g | sed s/\\/tdaq-v......../\\/tdaq-v_\ \ \ /g | tee m.txt; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=0; fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"; less m.txt'
#alias  mz='date; start_time=$(date +%s); spack concretize --force --deprecated; spack mpd build --clean -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=$((end_time - start_time)); fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"'
# When using the fork of spack-mpd
alias  mb='date; start_time=$(date +%s); spack find | grep gcc; spack mpd build -G Ninja -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g | sed s/\\[padded-to-255-chars\\]//g | sed s/\\/tdaq-v......../\\/tdaq-v_\ \ \ /g; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=$((end_time - start_time)); fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"'
alias  ml='date; start_time=$(date +%s); spack find | grep gcc; spack mpd build -G Ninja -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g | sed s/\\[padded-to-255-chars\\]//g | sed s/\\/tdaq-v......../\\/tdaq-v_\ \ \ /g | tee m.txt; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=0; fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"; less m.txt'
alias  mz='date; start_time=$(date +%s); spack concretize --force --deprecated; spack mpd build -G Ninja --clean -j$CETPKG_J 2>&1 | sed s/__spack_path_placeholder__//g; end_time=$(date +%s); pushd /home/nfs/emdaq/andres_develop/build; ninja install; popd; date; delta_time=$((end_time - start_time)); fractional_minutes=$(echo "scale=1; $delta_time / 60" | bc); echo "Full time: $delta_time seconds or $fractional_minutes minutes"'

spack load otsdaq-utilities

echo
echo -e "setup_ots.sh:${LINENO} |  \t  Now use 'ots --wiz' to configure otsdaq"
echo -e "setup_ots.sh:${LINENO} |  \t           Then use 'ots' to start otsdaq"
echo -e "setup_ots.sh:${LINENO} |  \t           Or use 'ots --help' for more options"
echo
echo -e "setup_ots.sh:${LINENO} |  \t      use 'kx' to kill otsdaq processes"
echo

echo -e "setup_ots.sh:${LINENO} |  \t  "
echo -e "setup_ots.sh:${LINENO} |  \t      setup_ots.sh creates some compiling aliases for you:"
echo -e "setup_ots.sh:${LINENO} |  \t     ---------------"
echo -e "setup_ots.sh:${LINENO} |  \t            mb                             ### for incremental build"
echo -e "setup_ots.sh:${LINENO} |  \t            mz                             ### for clean build"
echo -e "setup_ots.sh:${LINENO} |  \t     ---------------"
echo -e "setup_ots.sh:${LINENO} |  \t  "
echo -e "setup_ots.sh:${LINENO} |  \t  "
