from Camera import *
from Ray import *
from Parser import *
from MathUtils import *
from primUtils import *
from RayTracingUtils import *
import os
import keyboard
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
	target = stat[3]
	objects = stat[4]
	tObj = None
	render = True

	for obj in objects:
		if obj.label == target:
			tObj = obj
	
	out = Image.new("RGB", (imgWidth, imgHeight))

	while True:
		if render == True:
			for i in range(imgWidth):
				for j in range(imgHeight):
					out.putpixel((i,j), (0,0,0))
			print("Rendering")
			for i in range(imgWidth):
				for j in range(imgHeight):
					ray = makeRay(camera, imgWidth, imgHeight, i, j, FOV)
					intersects = list()
					for obj in objects:
						if obj.id == "Mesh":
							for child in obj.children:
								hit = child.isHit(transformRay(ray, obj.local()))
								if hit[0] == True:
									intersects.append(hit[1])

						else:
							hit = obj.isHit(transformRay(ray, obj.local()))
							if hit[0] == True:
								intersects.append(hit[1])

					#print(intersects)
					if len(intersects) > 0:
						vis = min(intersects, key= lambda t: t[0])
						out.putpixel((i,j), vis[1])

				render = False
			out.save(outFile)
			os.system(f"firefox {outFile}")

		if tObj != None:
			key = input()
			if key == "c":
				exit()
			elif key == "w":
				translateP(2.5,0,0, tObj)
				render = True

			elif key == "a":
				rotatePY(numpy.pi/4, tObj)
				render = True

			elif key=="d":
				rotatePY(-numpy.pi/4, tObj)
				render = True

			elif key == "s":
				translateP(-2.5,0,0, tObj)
				render = True
		else:
			break
	out.close()
