#!/bin/bash

SENSOR=$HOME/pi3/bin/DHT
SENSOR_BOARD_PIN=3

[ -f $SENSOR ] || exit 1

tries=0
while ((tries++ < 5))
do
	res=$(sudo $SENSOR board $SENSOR_BOARD_PIN)
	if [ $? -eq 0 ] 
	then
		echo $res
		exit 0
	else
		sleep 2
	fi
done

exit 1
