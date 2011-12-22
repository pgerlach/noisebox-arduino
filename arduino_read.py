#!/usr/bin/python

import os, sys
import serial
import urllib,re

ttyArduino = "/dev/ttyACM0"

se = serial.Serial(ttyArduino, 9600)

baseUrl = 'http://localhost:4242/'

while True:
	line = se.readline();
	line = (line.splitlines())[0];
	line.strip()
	print 'line read : %s' % (line)
	vmatch = None

	path = ''

	vMatch = re.match('rfid (\w+)', line)
	if ('playpause' == line):
		path = 'cmd/playpause'
	elif ('prev' == line):
		path = 'cmd/prev'
	elif ('next' == line):
		path = 'cmd/next'
	elif ('vminus' == line):
		path = 'cmd/vminus'
	elif ('vplus' == line):
		path = 'cmd/vplus'
	elif (None != vMatch):
		path = 'tag/' + vMatch.group(1)

	print 'path: ' + path
	try:
		urllib.urlopen(baseUrl + path)
	except:
		pass
