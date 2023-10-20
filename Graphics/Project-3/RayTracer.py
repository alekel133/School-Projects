from Camera import *
from Ray import *
from Parser import *
from MathUtils import *
from primUtils import *
import numpy

if __name__ == "__main__":
	if(len(argv) < 3):
		raise SyntaxError("Usage:<input filename> <output filename>")
	inFile = argv[1]
	outFile = argv[2]
	stat = ParseSDL(inFile)
	imgWidth = stat[0][0]
	imgHeight = stat[0][1]
	FOV = stat[1]
	camera = stat[2]
	objects = stat[3]
	print((imgWidth, imgHeight), FOV, camera, end="")
	for obj in objects:
		print(obj)