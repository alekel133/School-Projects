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

	def isHit(self, ray):
		direction = ray.dir
		origin = ray.origin
		center = self.center
		radius = self.radius
		color = self.color
		b = numpy.dot(direction, origin)
		c = numpy.dot(origin,origin) - radius**2
		disc = b**2 - c
		if(disc < 0):
			return (False, None)

		t0 = -b-numpy.sqrt(disc)
		t1 = -b + numpy.sqrt(disc)

		if(t1 < 0):
			return (False, None)
		if(t0 < 0):
			return(True, (t1, color))
		if(t1 != t0):
			return(True, (min(t1, t0), color))

		return(True, (t0, color))


	def getChildStr(self):
		out = ""
		for child in self.children:
			out += str(child)
			out += " | "

	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.parent}, Center={transform(self.center, self.inv())}, Radius={self.radius}"

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
	def isHit(self, ray):
		eye = ray.origin
		direction = ray.dir
		normal = self.normal

		top = numpy.dot(-eye, normal)
		bottom = numpy.dot(direction, normal)

		if bottom == 0:
			return(False, None)

		t = top/bottom

		if(t < 0):
			return(False, None)

		return (True, (t, self.color))

	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.parent}, Origin={transform(self.origin, self.inv())}, Normal={transform(self.normal, self.inv())}, Color= {self.color}"

class Triangle(Primitive):
	def __init__(self, P1: numpy.array, P2:numpy.array, P3:numpy.array, label: str, parent:str = None, color: tuple = (0,0,0)):
		s1 = P2 - P1
		s2 = P3 - P1
		self.p1 = numpy.append(P1,1)
		self.p2 = numpy.append(P2,1)
		self.p3 = numpy.append(P3,1)
		self.origin= numpy.array([0,0,0,1])
		self.normal = normalize(numpy.append(numpy.cross(s1, s2), 0))
		self.label = label
		self.parent = parent
		self. color = color
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	

	def __str__(self):
		return f"{self.label} (Triangle): Parent={self.parent}, Origin={transform(self.origin, self.inv())}, P1={transform(self.p1, self.inv())}, P2={transform(self.p2, self.inv())}, P3={transform(self.p3, self.inv())}, Normal={transform(self.normal, self.inv())}"


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
		return f"{self.label} (Mesh):Parent ={self.parent}"