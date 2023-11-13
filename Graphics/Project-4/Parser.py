"""
Author: Alexander Kellough
NetID: atk133

Description: Functions for parsing custom sdf and .obj files
"""
from sys import argv
import random as rand
from Camera import *
from Primitive import *
from primUtils import *

# Function for Generating Random Colors
def genRandColor():
	r = rand.randint(0,256)
	g = rand.randint(0,256)
	b = rand.randint(0,256)
	return (r, g, b)

# Finds object by label
def findParent(label, objects):
	for obj in objects:
		if obj.label == label:
			return obj

# Adds a child to a parent object
def addChild(child, objects):
	for obj in objects:
		if child.parent == None:
			return
		if child.parent.label == obj.label:
			print("True")
			obj.children.append(child)


# Applies transformations to an object
def move(val, p: Primitive, objects):
	trans = val.split(",")
	for t in trans:
		inst = t.split(":")
		if inst[0] == "T":
			coords = inst[1].split("/")
			x = float(coords[0])
			y = float(coords[1])
			z = float(coords[2])
			translateP(x, y, z, p)

		elif inst[0] == "RX":
			theta = degToRad(float(inst[1]))
			rotatePX(theta, p)

		elif inst[0] == "RY":
			theta = degToRad(float(inst[1]))
			rotatePY(theta, p)

		elif inst[0] == "RZ":
			theta = degToRad(float(inst[1]))
			rotatePZ(theta, p)


# Generates mesh from .obj file
def genMesh(filename, mesh, objects):
	file = open(filename)
	vertices = list() 
	count = 0
	for line in file:
		val = line.split()
		if val[0] == "#":
			continue

		elif val[0] == "v":
			vertices.append(numpy.array([float(val[1]), float(val[2]), float(val[3])]))

		elif val[0] == "f":
			t = Triangle(vertices[int(val[1])-1], vertices[int(val[2])-1], vertices[int(val[3])-1], f"{mesh.label}-Traingle-{count}", mesh, genRandColor())
			mesh.children.append(t)
			count += 1

# Parses SDL File (NOTE: These rules are not strictly enforced, some throw appropriate errors, some do not.)
# FOR GRADER: See attached files scene.sdf and tetrahedron.sdf for examples on usage in the .sdf file format
def ParseSDL(filename):
	imgHeight = 100
	imgWidth = 100
	FOV = numpy.pi/2
	camera = Camera()
	objects = list()
	target = None
	file = open(filename)

	count = 0
	for line in file:
		count = count+1
		val = line.split()
		if len(val) == 0:
			break

		if val[0][0] == "#":
			continue

		#Main Parsing Functions
		if val[0] == "Image:": # Get Image Dimensions
			if len(val) < 3:
				raise SyntaxError(f"(SDF) On Line {count}. No Images Dimensions Specified.")

			imgWidth = int(val[1])
			imgHeight = int(val[2])

		elif val[0] == "FOV":
			if len(val) < 2:
				raise SyntaxError(f"(SDF) On Line {count}. No FOV speified")

			FOV = float(val[1]) * numpy.pi/180

		elif val[0] == "Camera:":
			if len(val) < 2:
				continue
			move(val[1], camera, objects)

		elif val[0] == "Sphere:":
			label = val[1].strip("()")
			parent = findParent(val[2].strip("()"), objects)
			if(parent == "None"):
				parent = None

			radius = float(val[3])
			if radius <= 1:
				radius += 1
			s = Sphere(label, parent, radius, genRandColor())
			if len(val) == 5:
				move(val[4], s, objects)
			print(val[4])
			addChild(s, objects)
			objects.append(s)

		elif val[0] == "Plane:":
			label = val[1].strip("()") 
			parent = findParent(val[2].strip("()"), objects)
			if(parent == "None"):
				parent = None

			p = Plane(label, parent, genRandColor())
			if len(val) == 4:
				move(val[3], p, objects)

			addChild(p, objects)
			objects.append(p)

		elif val[0] == "Mesh:":
			label = val[1].strip("()") 
			parent = findParent(val[2].strip("()"), objects)
			tmp = list()
			if(parent == "None"):
				parent = None

			m = Mesh(label, parent)
			if len(val)==5:
				move(val[4], m, objects)
			genMesh(val[3],m,objects)
			objects.append(m)

		elif val[0] == "Target:":
			target = val[1]


	file.close()
	return ((imgWidth, imgHeight), FOV, camera, target, objects)

# If file is run as main, parses .sdf file and prints data to screen
if __name__ == "__main__":
	if(len(argv) < 2):
		raise ValueError("argv[1]: Required Argument <filename> not provided")
	filename = argv[1]
	stat = ParseSDL(filename)
	imgWidth = stat[0][0]
	imgHeight = stat[0][1]
	FOV = stat[1]
	camera = stat[2]
	objects = stat[3]
	print((imgWidth, imgHeight), FOV, camera, end="")
	for obj in objects:
		print(obj)
