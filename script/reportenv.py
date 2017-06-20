#!/usr/bin/env python

import sys
import subprocess
import httplib, urllib
from datetime import datetime
import time

URL = "http://10.0.0.7:8000/smarthome/api/upload"
MAX_PENDING_SIZE = 96*7
SAMPLE_INTERVAL = 900 # how many seconds to sleep between posts to server

def read_temperature():
    p = subprocess.Popen("/home/pi/pi3/script/temperature.sh", \
                         stdout = subprocess.PIPE, shell = True)
    (out, err) = p.communicate()
    if out:
        out = out.split()
        if (len(out) != 4):
            print date, 'read temperature fail: error'
            return (None,None)
        return (out[1],out[3])
    else:
        print date, 'read temperature fail: no response'
        return (None,None)

class Sample:
    def __init__(self):
        self.date = datetime.now().strftime("%Y-%m-%d %H:%M")
        self.home = 'changnin'
        self.temperature, self.humidity = read_temperature()

    def valid(self):
        return self.temperature is not None

    def data(self):
        if not valid(): return None
        return {'date':self.date, 'home':self.home, \
                'temperature':self.temperature, 'humidity':self.humidity }

#Report data to server
def upload(url, data):
    root_url = url.split('/')[0]
    sub_url = '/' + '/'.join(url.split('/')[1:])

    conn = httplib.HTTPConnection(root_url, timeout=10)

    headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}

    params = urllib.urlencode(data)
    try:
        conn.request("POST", sub_url, params, headers)
        response = conn.getresponse()
        print date, response.status, response.reason
        #res = response.read()
        conn.close()
        return True
    except:
        conn.close()
        print "failed to upload " + str(data)
        return False

if __name__ == "__main__":
    pendinglist = list()
    while True:
        data = Sample().data()
        if not data: continue

        if upload(URL, data):
            # resend pending when network recovers
            failed = list()
            for data in pendinglist:
                if not upload(URL, data):
                    failed.append(data)
            pendinglist = failed
        else:
            pendinglist.append(data)
            if len(pendinglist) > MAX_PENDING_SIZE:
                del pendinglist[0]
        if pendinglist:
            print 'pending list length ' + str(len(pendinglist))
        time.sleep(SAMPLE_INTERVAL)

