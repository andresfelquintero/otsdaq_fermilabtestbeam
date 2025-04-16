#!/bin/bash

do_update=0
do_firefox=0
do_run=0

env_opts_var=`basename $0 | sed 's/\.sh$//' | tr 'a-z-' 'A-Z_'`_OPTS
USAGE="\
   usage: `basename $0` [options]
examples: `basename $0` 
          `basename $0` --update
	  `basename $0` --run --firefox
--help        This help message
--update      Update this script and the otsdaq_readme.txt help file
--run         Run FTBF DAQ
--firefox     Start FireFox Web Browser, pointing to FTBF DAQ Desktop
--experiment  Use the given name for storing data files (default `date +%Y_%m_%B`_FTBF)
"

# Process script arguments and options
eval env_opts=\${$env_opts_var-} # can be args too
eval "set -- $env_opts \"\$@\""
op1chr='rest=`expr "$op" : "[^-]\(.*\)"`   && set -- "-$rest" "$@"'
op1arg='rest=`expr "$op" : "[^-]\(.*\)"`   && set --  "$rest" "$@"'
reqarg="$op1arg;"'test -z "${1+1}" &&echo opt -$op requires arg. &&echo "$USAGE" &&exit'
args= do_help= 
while [ -n "${1-}" ];do
    if expr "x${1-}" : 'x-' >/dev/null;then
        op=`expr "x$1" : 'x-\(.*\)'`; shift   # done with $1
        leq=`expr "x$op" : 'x-[^=]*\(=\)'` lev=`expr "x$op" : 'x-[^=]*=\(.*\)'`
        test -n "$leq"&&eval "set -- \"\$lev\" \"\$@\""&&op=`expr "x$op" : 'x\([^=]*\)'`
        case "$op" in
            \?*|h*)     eval $op1chr; do_help=1;;
            -help)      eval $op1arg; do_help=1;;
	    -update)    do_update=1;;
	    -run)       do_run=1;;
	    -firefox)   do_firefox=1;;
	    -experiment) eval $reqarg; export OTSDAQ_RUN_NAME=$1; shift;;
            *)          aa=`echo "-$op" | sed -e"s/'/'\"'\"'/g"` args="$args '$aa'";
        esac
    else
        aa=`echo "$1" | sed -e"s/'/'\"'\"'/g"` args="$args '$aa'"; shift
    fi
done
eval "set -- $args \"\$@\""; unset args aa

test -n "${do_help-}" && echo "$USAGE" && exit

if [ $do_update -eq 1 ];then
    scp ftbf-daq-08:/data/otsdaq_ftbf_user/srcs/otsdaq_fermilabtestbeam/tools/setup_otsdaq.sh .
    scp ftbf-daq-08:/data/otsdaq_ftbf_user/srcs/otsdaq_fermilabtestbeam/otsdaq_readme.txt .
    exit
    return
fi

if [[ "$HOSTNAME" != "ftbf-daq-08.fnal.gov" ]]; then
    args=""
    if [ $do_run -eq 1 ]; then
	args="--run"
    fi
    ssh ftbf-daq-08.fnal.gov /home/nfs/ftbf_user/Desktop/setup_otsdaq.sh $args &
else
    source /data/otsdaq_ftbf_user/setup_fermilabtestbeam.sh

    if [ $do_run -eq 1 ]; then
	StartOTS.sh -w
	StartOTS.sh

	ssh ftbf-cr-08 OTSDAQ_RUN_NAME=$OTSDAQ_RUN_NAME /data/otsdaq_ftbf_user/setup_fermilabtestbeam.sh run &
    fi
fi

if [ $do_firefox -eq 1 ];then
    echo "Running firefox https://ftbf-daq-08.fnal.gov"
    firefox https://ftbf-daq-08.fnal.gov/ &
else
    echo "FTBF DAQ has been started, navigate to https://ftbf-daq-08.fnal.gov to access OTSDAQ Desktop"
fi
