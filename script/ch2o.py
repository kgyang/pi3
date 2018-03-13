#!/usr/bin/env python

import sys 
import time 
import serial

def read_ch2o():
    port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=0)
    retries = 0;
    while retries < 5:
        raw = port.read(9)
        if len(raw) == 9: break
        retries += 1
        time.sleep(1)
    if (retries >= 5): return -1
    #if (retries > 0): print 'retry times: ' + str(retries)
    data = [ord(x) for x in raw]
    if data[0] != 0xFF:
        sys.stderr.write('invalid header\n')
        return -1
    if data[1] != 0x17:
        sys.stderr.write('invalid gas type format\n')
        return -1
    if data[2] != 0x04:
        sys.stderr.write('invalid gas consentration unit\n')
        return -1
    if data[3] != 0x04:
        sys.stderr.write('invalid decimal digits\n')
        return -1
    range_hi = data[6]
    range_lo = data[7]
    measurement_range = range_hi*256 + range_lo
    if measurement_range != 1000:
        sys.stderr.write('invalid measurement range ' + str(measurement_range) + '\n')
        return -1

    chksum = data[8]
    calsum = 0
    for i in range(1,8):
        calsum += data[i]
    calsum = calsum%256
    calsum = (~calsum) + 1
    if (calsum < 0): calsum += 256
    if calsum != chksum:
        sys.stderr.write('check sum mismatch\n')
        return -1

    #valid data, now calculate gas consentration
    hi = data[4]
    lo = data[5]
    val = (hi*256 + lo)/10000.0
    return val

if __name__ == '__main__':
    gas = read_ch2o()
    print 'ch2o ' + str(gas)
