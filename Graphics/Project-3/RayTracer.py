from Camera import *
from Ray import *
from Parser import *
from MathUtils import *
from primUtils import *
from RayTracingUtils import *
import numpy
from PIL import Image

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
	
	out = Image.new("RGB", (imgWidth, imgHeight))

	for i in range(imgWidth):
		for j in range(imgHeight):
			ray = makeRay(camera, imgWidth, imgHeight, i, j, FOV)
			for obj in objects:
				hit = obj.isHit(transformRay(ray, obj.mat))
				if hit[0] == True:
					out.putpixel((i,j), hit[1][1])

out.show()
out.save(outFile)
out.close()
