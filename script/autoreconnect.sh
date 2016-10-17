#!/bin/bash

while true
do
	ifconfig wlan0 | grep inet >> /dev/null || sudo ifup --force wlan0
	sleep 300
done
