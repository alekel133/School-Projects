from Ray import *
from Primitive import Primitive
from MathUtils import *
import numpy
import math

class Camera(Primitive):
	def __init__ (self): 
		self.parent = None
		self.eye = numpy.array([0,0,0,1])
		self.dir = normalize(numpy.array([0,0,-1,0]))
		self.up = normalize(numpy.array([0,1,0,0]))
		self.side = normalize(numpy.array([1,0,0,0]))
		self.mat = numpy.array((self.side, self.up, -self.dir,[0,0,0,1]))


	def inv(self):
		return numpy.linalg.inv(self.mat)

	def __str__(self):
		inv = self.inv()
		return f"Eye: {transform(self.eye, inv)}, View-Direction: {-transform(self.dir, inv)}, Up: {transform(self.up, inv)}, Side: {transform(self.side, inv)}"
