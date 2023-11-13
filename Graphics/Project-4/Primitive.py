import numpy
from MathUtils import *
from Ray import *

# Base class
class Primitive():
	def __init__(self):
		self.parent = None
		self.childre = list()
		self.id = "Primitive"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))

    def getBounds(self):
        inf = float("inf")
        return (numpy.array([0,0,0,1]), numpy.array([inf, inf, inf, 1]))
    
	# Virtual isHit function
	# NOTE: Should be overwritten in any derived classes
	def isHit(self, ray):
		return (False, None)

	# Returns child str
	def getChildStr(self):
		out = ""
		for child in self.children:
			out += str(child)
			out += " | "

	def inv(self):
		return numpy.linalg.inv(self.mat)

	# Recursively iterates through parent objects, and returns the composition
	# of their transformation matrices
	def local(self):
		if self.parent == None:
			return self.mat
		return transform(self.parent.local(), self.mat)

	# Local but reversed to return to world coords
	def world(self):
		if self.parent == None:
			return self.mat
		return transform(self.inv(), self.parent.inv())

	# Returns the label of the parent for output
	def getParentLabel(self):
		if self.parent == None:
			return "None"
		return self.parent.label

class Sphere(Primitive):
	def __init__(self, label:str, parent:Primitive = None, radius:float = 1, color: tuple = (0,0,0)):
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
		"""
		The following code is taken from the notes on meshes by Dr. Jankun-Kelly.
		As well as being modified/changed to follow my calculations and semantics.
		"""
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

		t0 = -b - numpy.sqrt(disc)
		t1 = -b + numpy.sqrt(disc)

		if(t1 < 0):
			return (False, None)
		if(t0 < 0):
			return(True, (t1, color))
		if(t1 != t0):
			return(True, (min(t1, t0), color))

		return(True, (t0, color))

	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.getParentLabel()}, Center={transform(self.center, self.world())}, Radius={self.radius}"

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

	# Chechks if ray is parallel to plane, if not
	# If not, calculates t, and returns the portion in front of the camera
	def isHit(self, ray):
		eye = ray.origin
		direction = ray.dir
		normal = self.normal

		top = numpy.dot(self.origin-eye, normal)
		bottom = numpy.dot(direction, normal)

		if bottom == 0:
			return(False, None)

		t = top/bottom

		if(t <= 0):
			return(False, None)

		return (True, (t, self.color))

	def __str__(self):
		return f"{self.label} ({self.id}): Parent={self.getParentLabel()}, Origin={transform(self.origin, self.world())}, Normal={transform(self.normal, self.world())}, Color= {self.color}"

class Triangle(Primitive):
	def __init__(self, P1: numpy.array, P2:numpy.array, P3:numpy.array, label: str, parent:str = None, color: tuple = (0,0,0)):
		s1 = P2-P1
		s2 = P3-P1
		self.p1 = numpy.append(P1,1)
		self.p2 = numpy.append(P2,1)
		self.p3 = numpy.append(P3,1)
		self.origin= numpy.array([0,0,0,1])
		self.normal = normalize(numpy.append(numpy.cross(s1, s2), 0))
		self.label = label
		self.parent = parent
		self.color = color
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))	

	# The following code is modified from https://github.com/jonnovak/ray triangle-test/blob/master/python
	# Calculates barycentric coordinates
	def isHit(self, ray):
		eye = ray.origin
		direction = ray.dir
		p1 = self.p1
		p2 = self.p2
		p3 = self.p3

		# Calculate edges from p1
		AB = p2-p1
		AC = p3-p1

		# Project the direction onto AC
		pvec = numpy.append(numpy.cross(direction[:3], AC[:3]),1)

		# Calculate the denominator of the Barycentric Coordinate equation (Edge x Intersection Edge)/(EdgeAB x EdgeAC)
		det = numpy.dot(AB, pvec)

		# If this is zero, the plane the triangle sits on is parallel to the ray direction, so return false
		if det < 0:
			return (False, None)

		# Since we have to divide by the value, invert it (multiplication is faster than division)
		invDet = 1/det

		# Get the vector from A to eye
		tvec = eye - p1

		# Get the area of this new triangle and divide it by the total area to get beta (u)
		u = numpy.dot(tvec,pvec)*invDet
		if u < 0 or u > 1:
			return (False, None)

		# Repeat previous steps to get value for gamma (v)
		qvec = numpy.append(numpy.cross(tvec[:3], AB[:3]), 1)
		v = numpy.dot(direction, qvec) * invDet

		# The barycentric coordinates have to be positive, and add up to less than one. The alpha coordinate is left of, because
		# It is solved for like this: alpha = 1-beta-gamma
		if v < 0 or u + v > 1:
			return (False, None)

		# Get the t value
		t = numpy.dot(AC, qvec) * invDet
		return (True, (t, self.color))

	def __str__(self):
		return f"{self.label} (Triangle): Parent={self.getParentLabel()}, Origin={transform(self.origin, self.world())}, P1={transform(self.p1, self.world())}, P2={transform(self.p2, self.world())}, P3={transform(self.p3, self.world())}, Normal={transform(self.normal, self.world())}"


# A list for holding triangles
# Made from Primitive base class, so that sub-meshes
# Will transform in sync
class Mesh(Primitive):
	def __init__(self, label:str, parent: Primitive = None):
		self.origin = numpy.array([0,0,0,1])
		self.label = label
		self.parent = parent
		self.children = list()
		self.id = "Mesh"
		self.mat = numpy.array(([1,0,0,0],
							  [0,1,0,0],
							  [0,0,1,0],
							  [0,0,0,1],))

	def __str__(self):
		return f"{self.label} (Mesh):Parent ={self.getParentLabel()}, Origin {transform(self.origin, self.world())}"

class AABB(Primitive):
    def genBox(self, objects: list):
        inf = float("inf")
        minPoint = numpy.array([0,0,0,1])
        maxPoint = numpy.array([inf, inf, inf, 1])
        for obj in objects:
            minPoint = obj.getBounds
            
    def __init__(self, label: str= "None", objects: list, parent: Primitive = None):
        self.label = label
        self.parent = parent
        gen

    def isHit(self, ray:Ray):
        dirInv = 1/ray.direction
        tMin = 0.0
        tMax = float("inf")
        
        for d in range(3):
            t1 = self.minPoint[d] - ray.origin[d] * dirInv
            t2 = self.maxPoint[d] - ray.origin[d] * dirInv

            tMin = max(tMin, min(t1, t2))
            tMax = min(tMax, max(t1, t2))

        return tMin < tMax

