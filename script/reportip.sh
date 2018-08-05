#!/bin/bash

IP_FILE='/home/pi/ext.ip'

ip=$(curl -s -m 5 'http://ipecho.net/plain')

[[ -n "$ip" ]] || exit 1

old=''
[[ -f ${IP_FILE} ]] && old=$(tail -1 ${IP_FILE} | awk '{print $NF}')

[[ "$old" == "$ip" ]] || {
	time=$(date +%F_%H-%M-%S)
	/home/pi/pi3/script/sendmail.py -s 'External IP' $MAIL_USER <<< $ip && echo "$time $ip" >> ${IP_FILE}
}
