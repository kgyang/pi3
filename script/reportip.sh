#!/bin/bash

IP_FILE='/home/pi/ext.ip'

ip=$(curl -s -m 5 'http://ipecho.net/plain')

[[ -n "$ip" ]] || exit 1

old=''
[[ -f ${IP_FILE} ]] && old=$(cat ${IP_FILE})

[[ "$old" == "$ip" ]] || {
	/home/pi/pi3/script/sendmail.py -s 'External IP' $MAIL_USER <<< $ip && echo $ip > ${IP_FILE}
}
