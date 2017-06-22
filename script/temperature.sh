#!/bin/bash

SENSOR=/home/pi/pi3/bin/DHT
SENSOR_BOARD_PIN=3
RETRY_TIMES=8

[ -f $SENSOR ] || exit 1

tries=0
while ((tries++ < $RETRY_TIMES))
do
	res=$($SENSOR board $SENSOR_BOARD_PIN)
	if [ $? -eq 0 ] 
	then
		echo $res
		exit 0
	else
		sleep 2
	fi
done

exit 1
