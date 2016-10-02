#!/bin/bash

LOG_DIR=$HOME/log
LOG_FILE=temperature.log
SCRIPT_DIR=$(dirname $0)

mkdir -p $LOG_DIR

logfile=$LOG_DIR/$LOG_FILE

[ -f $logfile ]  && {
	lines=$(wc -l $logfile | awk '{print $1}')
	((lines <= 1000)) || exit 1
}

res=$(${SCRIPT_DIR}/temperature.sh)

[ $? -eq 0 ] && echo "$(date) ${res}" >> $logfile

${SCRIPT_DIR}/generatetemperatepage.sh $logfile
