#!/usr/bin/python

import re
import matplotlib.pyplot as plt
import numpy as np
import json


#SAMPLE_FILE='/home/pi/env_sample_data.txt'
SAMPLE_FILE='env_sample_data.txt'

def read_env_data(filename):
	data = list()
	with open(filename, 'r') as f:
		for line in f:
			line = re.sub("'", '"', line) # replace ' with "
			line = re.sub("None", '0', line) # replace None with 0
			data.append(json.loads(line))
	return data

def plot_m(ax, data, metric, title=''):
	for d in data:
		if metric not in d: d[metric] = "0"
	do_plot(ax, [ d['date'] for d in data ],[ float(d[metric]) for d in data ], metric, title)

def do_plot(ax, x, y, label, title):
	ax.plot(x, y, label=label)
	ax.set(xlabel='time', ylabel=label, title=title)
	ax.grid()
	#ax.legend()

def plot_latest(axes, data, hour):
	data = data[-hour*2:]
	for d in data: d['date'] = d['date'].split()[1]
	plot_m(axes[0], data, 'temperature', 'Home env monitor')
	plot_m(axes[1], data, 'humidity')
	plot_m(axes[2], data, 'ch2o')

fig, axes = plt.subplots(3, 1)

data = read_env_data(SAMPLE_FILE)

plot_latest(axes, data, 6)


plt.show()
#fig.savefig('env.png')

