#!/bin/bash

while true
do
	ifconfig wlan0 | grep inet >> /dev/null || {
		echo "$(date): wlan0 down, redo ifup" >> /tmp/autoreconnect.log
		sudo ifup --force wlan0
	}
	sleep 300
done

