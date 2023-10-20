import numpy
from Camera import *
from Objects import *
from Ray import *

def makeRay(camera: Camera, iWidth: int, iHeight: int, i:int, j:int, fov = numpy.pi/2, ortho:bool = False):
    inv = camera.inv()
    origin = camera.eye
    up = camera.up
    side = camera.side
    d = -camera.dir
    pc = origin + d
    ap = iWidth/iHeight

    ihat = numpy.cross(up[:3], d[:3])

    pc = numpy.array([iWidth/2, iHeight/2, ])
def transformRay(ray: Ray, mat: numpy.array):
    dir = transform(ray.dir, mat)
    eye = transform(ray.origin, mat)
    return Ray(eye, dir)