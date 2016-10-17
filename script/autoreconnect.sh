#!/bin/bash

while true
do
if ifconfig wlan0 | grep inet
then
	sleep 60
else
	sudo ifup --force wlan0
	sleep 10
fi
done
