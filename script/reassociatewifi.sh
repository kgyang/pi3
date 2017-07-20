#!/bin/bash


#wpa_cli status
#Selected interface 'wlan0'
#bssid=c4:3d:c7:9c:59:a6
#freq=2447
#ssid=NETGEAR-JT-YF
#id=0
#mode=station
#pairwise_cipher=CCMP
#group_cipher=TKIP
#key_mgmt=WPA2-PSK
#wpa_state=COMPLETED
#ip_address=10.0.0.80
#p2p_device_address=b8:27:eb:1e:6f:10
#address=b8:27:eb:1e:6f:10
#uuid=fd826de2-617f-5b12-b427-a91a358041c6

get_wpa_state()
{
	echo -n $(wpa_cli status | sed -n 's/wpa_state=\(.*\)/\1/p')
}

if [ "$(get_wpa_state)" == "SCANNING" ]
then
	wpa_cli reassociate
fi

