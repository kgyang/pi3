#!/bin/bash

ifconfig wlan0 | grep inet >> /dev/null || sudo ifup --force wlan0
