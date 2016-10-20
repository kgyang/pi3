#!/bin/bash

log()
{
	log_file=/tmp/autoreconnect.log
	echo "$(date): $1" >> $log_file
}

consecutive_failure=0
while ((consecutive_failure < 100))
do
	if ifconfig wlan0 | grep 'inet addr' >> /dev/null
	then
		consecutive_failure=0
	else
		let consecutive_failure++
		log "wlan0 down, redo ifup"
		sudo ifup --force wlan0
	fi
	sleep 300
done
log "failure"

