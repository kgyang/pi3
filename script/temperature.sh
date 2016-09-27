#!/bin/bash

DATA_DIR=/tmp/$LOGNAME
DATA_FILE=temeprature_data.log
EXE=/home/pi/pi3/bin/DHT

mkdir -p $DATA_DIR
[ -f $DATA_DIR/$DATA_FILE ]  && {
	lines=$(wc -l $DATA_DIR/$DATA_FILE | awk '{print $1}')
	((lines <= 1000)) || exit 1
}

[ -f $EXE ] || {
	cd /home/pi/pi3
	./buildall.sh
}
tries=0
while ((tries++ < 5))
do
	res=$(sudo $EXE)
	if [ $? -eq 0 ] 
	then
		echo "$(date) ${res}" | tee -a ${DATA_DIR}/${DATA_FILE}
		break
	else
		sleep 2
	fi
done

exit 0
