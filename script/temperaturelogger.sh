#!/bin/bash

LOG_DIR=/tmp/$LOGNAME
LOG_FILE=temperature.log

mkdir -p $LOG_DIR

logfile=$LOG_DIR/$LOG_FILE

[ -f $logfile ]  && {
	lines=$(wc -l $logfile | awk '{print $1}')
	((lines <= 1000)) || exit 1
}

cd $(dirname $0)

res=$(./temperature.sh)

[ $? -eq 0 ] && echo "$(date) ${res}" >> $logfile

