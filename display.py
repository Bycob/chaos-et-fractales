# -*- coding: utf-8 -*-

import threading
import sys
import time
import numpy as np
import matplotlib.pyplot as plt

#TODO faire compatibilité entre python 2.7 et python 3.5

plotLines = False;

def plot_from_file(filename, color) :
	fileContent = open(filename, 'r')
	lines = fileContent.readlines()

	listX = list()
	listY = list()

	for line in lines :
		element = line.split(";")
		listX.append(float(element[1]))
		listY.append(float(element[2]))
	
	global plotLines
	if plotLines :
		plt.plot(listX, listY, color=color)
	else :
		plt.scatter(listX, listY, color=color)

def plot_from_cin() :

	plt.axis('equal')
	plt.ion()
	plt.show()
	buffersX = dict()
	buffersY = dict()
	plots = dict()
	sleepCount = 0

	for input_line in sys.stdin :
		name, values = input_line.split(":")
		elements = values.split(";")
		
		if not plots.has_key(name) :
			plots[name], = plt.plot([float(elements[1])], [float(elements[2])], 'r')
			buffersX[name] = list()
			buffersY[name] = list()
		else :
			line = plots[name]
			buffersX[name].append(float(elements[1]))
			buffersY[name].append(float(elements[2]))
			
			if len(buffersX[name]) > 40 :
				xdata = np.append(line.get_xdata(), buffersX[name])
				ydata = np.append(line.get_ydata(), buffersY[name])
				line.set_data(xdata, ydata)
				buffersX[name] = list()
				buffersY[name] = list()
				sleepCount += 1

		if sleepCount > len(plots) :
			plt.pause(0.01)
			sleepCount = 0
		
	plt.close()


if __name__ == '__main__' :

	if "--line" in sys.argv :
		plotLines = True

	if "--realtime" in sys.argv :
		plot_from_cin()
	else :
		plt.axis('equal')
		"""plot_from_file('earth_', 'r')
		plot_from_file('mars_', 'g')
		plot_from_file('sun_', 'b')
		plot_from_file('jupiter_', 'b')
		"""
		plot_from_file('body1_', 'g')
		plot_from_file('body2_', 'b')
		plot_from_file('body3_', 'r')

		plt.show()
