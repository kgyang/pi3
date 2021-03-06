#!/usr/bin/env python

import sys
import subprocess
import httplib, urllib
from datetime import datetime
import time
import re
import ch2o

SAMPLE_FILE='/home/pi/env_sample_data.txt'
SAMPLE_PENDING_FILE='/home/pi/env_sample_pending.txt'
URL = '/smarthome/api/upload'

def read_ch2o():
    return ch2o.read_ch2o()

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
        self.home = ''
        self.temperature, self.humidity = read_temperature()
        time.sleep(1) # add 1 second latency to reduce ch2o read failures, not sure if it works
        self.ch2o = read_ch2o()

    def valid(self):
        return self.temperature is not None

    def data(self):
        if not self.valid(): return None
        return {'date':self.date, 'home':self.home, \
                'temperature':self.temperature, 'humidity':self.humidity,
                'ch2o':self.ch2o }

#Report data to server
def upload(url, data):
    m = re.match(r'(.*://[^/]*)(.*)', url)
    root_url = m.group(1)
    sub_url = m.group(2)

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

def save_data(data):
    with open(SAMPLE_FILE, 'a') as f:
        f.write(str(data)+'\n')

def save_data_in_pending(data):
    pass

def upload_pending():
    pass

if __name__ == "__main__":
    data = Sample().data()
    if not data: sys.exit(0)
    save_data(data)

    #if upload(URL, data):
    #    upload_pending()
    #else:
    #    save_data_in_pending(data)

