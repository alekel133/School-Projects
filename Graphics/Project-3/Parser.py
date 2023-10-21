from sys import argv
import random as rand
from Camera import *
from Primitive import *
from primUtils import *

def genRandColor():
	r = rand.randint(0,256)
	g = rand.randint(0,256)
	b = rand.randint(0,256)
	return (r, g, b)

def findParentMat(label, objects):
	if  label == None:
		return numpy.array(([1,0,0,0],
					   	   [0,1,0,0],
					       [0,0,1,0],
					       [0,0,0,1]))

	for obj in objects:
		if obj.label == label:
			return obj.mat

	return numpy.array(([1,0,0,0],
					   [0,1,0,0],
					   [0,0,1,0],
					   [0,0,0,1]))

def addChild(child, objects):
	for obj in objects:
		if child.parent == obj.label:
			obj.children.append(child)


def move(val, p: Primitive, objects):
	trans = val.split(",")
	p.mat = transform(p.mat, findParentMat(p.parent, objects))
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
			t = Triangle(vertices[int(val[1])-1], vertices[int(val[2])-1], vertices[int(val[3])-1], f"{mesh.label}-Traingle-{count}, label", mesh.label)
			t.mat = transform(t.mat, mesh.mat)
			mesh.children.append(t)


def ParseSDL(filename):
	imgHeight = 100
	imgWidth = 100
	FOV = numpy.pi/2
	camera = Camera()
	objects = list()
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
			parent = val[2].strip("()")	
			if(parent == "None"):
				parent = None

			radius = float(val[3])
			s = Sphere(label, parent, radius, genRandColor())
			move(val[4], s, objects)
			addChild(s, objects)
			objects.append(s)

		elif val[0] == "Plane:":
			label = val[1].strip("()") 
			parent = val[2].strip("()")	
			if(parent == "None"):
				parent = None

			p = Plane(label, parent, genRandColor())
			move(val[3], p, objects)
			addChild(p, objects)
			objects.append(p)

		elif val[0] == "Mesh:":
			label = val[1].strip("()") 
			parent = val[2].strip("()")	
			tmp = list()
			if(parent == "None"):
				parent = None

			m = Mesh(label, parent)
			move(val[4], m, objects)
			genMesh(val[3],m,objects)
			objects.append(m)


	file.close()
	return ((imgWidth, imgHeight), FOV, camera, objects)



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
