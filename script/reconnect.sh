#!/bin/bash

ifconfig wlan0 | grep 'inet addr' >> /dev/null || sudo ifup --force wlan0
