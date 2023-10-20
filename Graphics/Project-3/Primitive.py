import numpy
from MathUtils import *
from Ray import *

class Primitive():
	def __init__(self):
		self.parent = None
		self.childre = list()
		self.id = "Primitive"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))

	def isHit(self, ray):
		return (False, None)

	def getChildStr(self):
		out = ""
		for child in self.children:
			out += str(child)
			out += " | "

	def inv(self):
		return numpy.linalg.inv(self.mat)

class Sphere(Primitive):
	def __init__(self, label:str, parent:str = None, radius:float = 1, color: tuple = (0,0,0)):
		self.center = numpy.array([0,0,0,1])
		self.label = label
		self.parent = parent
		self.children = list()
		self.radius = radius
		self.color = color
		self.id = "Sphere"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	

	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.parent}, Center={transform(self.center, self.inv())}, Radius={self.radius}, color={self.getChildStr()}"

class Plane(Primitive):
	def __init__(self, label:str, parent:str = None, color: tuple = (0,0,0)):
		self.normal = numpy.array([0, 1, 0, 0])
		self.origin = ([0,0,0,1])
		self.label = label
		self.parent = parent
		self.children = list()
		self.color = color
		self.id = "Plane"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	
	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.parent}, Origin={transform(self.origin, self.inv())}, Normal={transform(self.normal, self.inv())}, Color= {self.color}, Children={self.getChildStr()}"

class Triangle(Primitive):
	def __init__(self, P1: numpy.array, P2:numpy.array, P3:numpy.array, label: str, parent:str = None, color: tuple = (0,0,0)):
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	

	def __str__(self):
		return f"{self.label} (Triangle): Parent={self.parent}, Origin={transform(self.origin, self.inv())}, P1={transform(self.p1, self.inv())}, P2={transform(self.P2, self.inv())}, P3={transform(self.p3, self.inv())}, Normal={transform(self.normal, self.inv())}"


class Mesh(Primitive):
	def __init__(self, label:str, parent: Primitive = None):
		self.center = numpy.array([0,0,0,1])
		self.label = label
		self.parent = parent
		self.children = list()
		self.id = "Mesh"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	
	def __str__(self):
		return f"{self.label} (Mesh):Parent ={self.parent}, Childre={self.getChildStr()}"