#!/usr/bin/env python

import time 
import serial

def read_ch2o():
    port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=0)
    raw = port.read(9)
    retries = 0;
    while len(raw) != 9 and retries < 10:
        retries += 1
        time.sleep(1)
        raw = port.read(9)
    if (retries >= 10): return None
    if (retries > 0): print 'retry times: ' + str(retries)
    data = [ord(x) for x in raw]
    if data[0] != 0xFF:
        print 'invalid header'
        return None
    if data[1] != 0x17:
        print 'invalid gas type format'
        return None
    if data[2] != 0x04:
        print 'invalid gas consentration unit'
        return None
    if data[3] != 0x04:
        print 'invalid decimal digits'
        return None
    range_hi = data[6]
    range_lo = data[7]
    measurement_range = range_hi*256 + range_lo
    if measurement_range != 1000:
        print 'invalid measurement range ' + str(measurement_range)
        return None

    chksum = data[8]
    calsum = 0
    for i in range(1,8):
        calsum += data[i]
    calsum = calsum%256
    calsum = (~calsum) + 1
    if (calsum < 0): calsum += 256
    if calsum != chksum:
        print 'check sum mismatch'
        return None

    #valid data, now calculate gas consentration
    hi = data[4]
    lo = data[5]
    val = (hi*256 + lo)/10000.0
    return val

if __name__ == '__main__':
    print str(read_ch2o())
