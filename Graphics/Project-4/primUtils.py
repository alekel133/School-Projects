from Primitive import Primitive
from MathUtils import *
import numpy

def rotatePX(theta, p: Primitive):
	p.mat = rotateX(theta, p.mat)

def rotatePY(theta, p: Primitive):
	p.mat = rotateY(theta, p.mat)

def rotatePZ(theta, p: Primitive):
	p.mat = rotateZ(theta, p.mat)

def translateP(x, y, z, p: Primitive):
	p.mat = translate(x, y, z, p.mat)