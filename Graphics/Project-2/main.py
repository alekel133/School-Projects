import numpy
import sys
from PIL import Image
import random as rand
import Objects
import Ray

def genRandFloat():
    return rand.randint(0, 256)



def parseFile(filename, height, width):
    file = open(filename)

    objects = list()
    vertices = list()
    camera = None
    im = None

    while True:
        line = file.readline()
        if not line:
            break

        val = line.split()
        if len(val) == 0:
            break

        if val[0][0] == "#":

            continue

        elif val[0] == "v":
            vertices.append(numpy.array((float(val[1]),float(val[2]),float(val[3]))))

        elif val[0] == "im":
           im = (int(val[1]), int(val[2])) 


        elif val[0] == "f":
            objects.append(Objects.Triangle((vertices[int(val[1])-1], vertices[int(val[2])-1], vertices[int(val[3])-1]),
                                            (genRandFloat(), genRandFloat(), genRandFloat())))

        elif val[0] == "sp":
            center = numpy.array((float(val[1]), float(val[2]), float(val[3])))
            line = file.readline()
            val = line.split()
            if val[0] != "sr":
                exit("Error: Radius not specified after sphere center.")
            
            radius = float(val[1])
            objects.append(Objects.Sphere(center, radius, (genRandFloat(),genRandFloat(),genRandFloat())))

        elif val[0] == "pc":
            origin = numpy.array((float(val[1]), float(val[2]), float(val[3])))
            line = file.readline()
            val = line.split()
            if val[0] != "pn":
                exit("Error: Noraml not specified after plane origin.")
            
            normal = numpy.array((float(val[1]),float(val[2]),float(val[3])))
            objects.append(Objects.Plane(origin, normal, (genRandFloat(),genRandFloat(),genRandFloat())))

        elif val[0] == "cp":
            eye = numpy.array((float(val[1]), float(val[2]), float(val[3])))
            line = file.readline()
            val = line.split()
            if(val[0] != "cu"):
                exit("Error: No camera up direction given")
            
            up = numpy.array((float(val[1]), float(val[2]), float(val[3])))

            line = file.readline()
            val = line.split()
            if val[0] != "vp":
                exit("Error: No viewpoint give.")
            view = numpy.array((float(val[1]), float(val[2]), float(val[3])))

            line = file.readline()
            val = line.split()
            if(val[0] != "vd"):
                exit("Error: No view distance(aperture) given.")
            
            vd = float(val[1])
            camera = Ray.Camera(eye,up, view, vd)

        elif not line:
            break

    if camera == None:
        exit("Error: No camera data provided.")

    return (camera, objects, im)
    
def minKey(tup):
    return tup[0]

def main():

    if len(sys.argv) < 5:
        exit("Error (Syntax): <outfile name> <infile name> <height> <width>.")

    ## Load Data from file and command line
    outname = sys.argv[1]
    filename = sys.argv[2]
    height = int(sys.argv[3])
    width = int(sys.argv[4])

    stat = parseFile(filename, height, width)

    camera = stat[0]
    objects = stat[1]
    im = stat[2]
    print(camera)
    for obj in objects: 
        print(obj)

    outImage = Image.new("RGB", (width, height))

    for i in range(width):
        for j in range(height):
            outImage.putpixel((i,j), (0,0,0))

    for i in range(width):
        for j in range(height):
            hit = list()
            vis = tuple()
            ray = Ray.generateRay(camera, im[0], im[1], i, j,height, width)
            for obj in objects:
                intersect = obj.isHit(ray)
                if(intersect[0] == True):
                    hit.append(intersect[1])

            if(len(hit) != 0):
                vis = min(hit, key=minKey)
                outImage.putpixel((i,j), vis[1])

    outImage.show()
    outImage.save(outname, format="PNG")
    outImage.close()

main()
