#!/bin/bash

MAX_CONSECUTIVE_FAILURE=100
consecutive_failure=0
while ((consecutive_failure < $MAX_CONSECUTIVE_FAILURE))
do
	if ifconfig wlan0 | grep 'inet addr' >> /dev/null
	then
		consecutive_failure=0
	else
		let consecutive_failure++
		logger "$0: wlan0 down, redo ifup, failure $consecutive_failure"
		sudo ifup --force wlan0
	fi
	sleep 300
done
logger "$0: wlan0 consecutive failure exceed $MAX_CONSECUTIVE_FAILURE times, quit"

