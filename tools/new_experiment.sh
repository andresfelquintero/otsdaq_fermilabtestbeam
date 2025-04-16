#!/bin/bash

if [[ "x$1" == "x" || "$1" == "-h" || "$1" == "--help" ]]; then
    echo -e "\n\nError! No experiment folder name provided. Please give the experiment folder name as the first argument, for example:\n\n\t source new_experiment.sh YEAR_MONTH_MONTHNAME_TNUMBER \n\n"
    return 1
    exit 1
fi


#make directory if it does not exist
if [ -d $EXPERIMENT_DIR ]; then
    echo
    echo
    echo -e `date +"%h%y %T"` "newExperiment[${LINENO}]  \t Error: path (${EXPERIMENT_DIR}) already exists!\n\n"
    return 1
    exit 1
fi

source ~/Desktop/setup_otsdaq.sh;

StartOTS.sh -k #kill all things ots

export OTSDAQ_RUN_NAME=$1
EXPERIMENT_DIR=$OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME

echo -e "newExperiment[${LINENO}]: \t =============================================="
echo
echo -e "newExperiment[${LINENO}]: \t Creating a new experiment directory at $EXPERIMENT_DIR"
echo 

mkdir -p ${EXPERIMENT_DIR} || echo -e `date +"%h%y %T"` "newExperiment[${LINENO}]  \t Error: path (${EXPERIMENT_DIR}) does not exist and mkdir failed!\n\n"

echo -e "newExperiment[${LINENO}]: \t cp -r ${MRB_SOURCE}/otsdaq_fermilabtestbeam/DataFTBF $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/UserData"
#the folder may already exists (from StartOTS.sh -k actions)
mkdir -p $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/UserData
cp -r ${MRB_SOURCE}/otsdaq_fermilabtestbeam/DataFTBF/* $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/UserData/

mkdir -p $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/databases
mkdir -p $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/databases/filesystemdb
echo -e "newExperiment[${LINENO}]: \t cp -r ${MRB_SOURCE}/otsdaq_fermilabtestbeam/databases/filesystemdb/ftbf_db $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/databases/filesystemdb/db"
cp -r ${MRB_SOURCE}/otsdaq_fermilabtestbeam/databases/filesystemdb/ftbf_db $OTSDAQ_FTBF_RUNS_PATH/$OTSDAQ_RUN_NAME/databases/filesystemdb/db

return 1
exit 1

