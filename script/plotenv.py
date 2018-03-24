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

def plot_m(ax, data, metric):
	for d in data:
		if metric not in d: d[metric] = "0"
	do_plot(ax, [ d['date'] for d in data ],[ float(d[metric]) for d in data ], metric)


def do_plot(ax, x, y, label):
	#x = np.arange(0, len(y), 1)
	ax.plot(x, y, label=label)

def plot_latest_6h(ax, data):
	data = data[-12:]
	for d in data: d['date'] = d['date'].split()[1]
	plot_m(ax, data, 'temperature')
	plot_m(ax, data, 'humidity')
	plot_m(ax, data, 'ch2o')

fig, ax = plt.subplots()

data = read_env_data(SAMPLE_FILE)

plot_latest_6h(ax, data)

ax.set(xlabel='DateTime', ylabel='Env', title='Home Env Monitor')
ax.grid()
ax.legend()

plt.show()
#fig.savefig('env.png')

