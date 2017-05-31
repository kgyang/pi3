#!/usr/bin/env python

import sys
import subprocess
import httplib, urllib
import datetime
import time

sleep = 900 # how many seconds to sleep between posts to server
home = 'changnin'

def getTempHumidity():
    p = subprocess.Popen("/home/pi/pi3/script/temperature.sh", stdout = subprocess.PIPE, shell = True)
    (out, err) = p.communicate()
    if out:
        out = out.split()
        if (len(out) != 4):
            return None,None
        return out[1],out[3]
    else:
        return None,None

#Report Raspberry Pi temperature to server
def reportenv():
    date = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    temp, hum = getTempHumidity()
    if not temp:
        print date, 'failed to get temperature and humidity'
        return

    params = urllib.urlencode({'date':date, 'temperature': temp, 'humidity': hum, 'home':home }) 
    headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
    conn = httplib.HTTPConnection("10.0.0.7:8000", timeout=10)
    try:
        conn.request("POST", "/smarthome/api/upload", params, headers)
        response = conn.getresponse()
        print date, response.status, response.reason
        data = response.read()
        conn.close()
    except:
        print "connection failed"

#sleep for desired amount of time
if __name__ == "__main__":
        reportenv()
        #while True:
                #reportenv()
                #time.sleep(sleep)
