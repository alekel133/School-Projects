import numpy
from Camera import *
from Primitive import *
from Ray import *

def makeRay(camera: Camera, iWidth: int, iHeight: int, i:int, j:int, fov = numpy.pi/2):
    inv = camera.inv()
    origin = camera.eye
    up = camera.up
    side = camera.side
    d = -camera.dir
    pc = origin + d
    ap = iWidth/iHeight

    ihat = numpy.append(numpy.cross(up[:3], d[:3]),0)
    jhat = up

    pij = pc + ((((i + 0.5)/iWidth)-1/2) * ihat)*fov + ((((j+0.5)/iHeight)-1/2)*jhat)*fov

    return transformRay(generateRay(origin, pij), camera.inv())

def transformRay(ray: Ray, mat: numpy.array):
    dir = normalize(transform(ray.dir, mat))
    eye = transform(ray.origin, mat)
    return Ray(eye, dir)