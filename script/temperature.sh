#!/bin/bash

SENSOR=/home/pi/pi3/bin/DHT

[ -f $SENSOR ] || exit 1

tries=0
while ((tries++ < 5))
do
	res=$(sudo $SENSOR)
	if [ $? -eq 0 ] 
	then
		echo $res
		exit 0
	else
		echo $res >&2
		sleep 2
	fi
done

exit 1