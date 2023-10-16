import numpy

class Camera:
    def __init__(self, position:numpy.array, up:numpy.array, viewpoint: numpy.array, viewDistance: float):
        self.position = position
        self.view = viewpoint - position
        self.view = self.view / numpy.linalg.norm(self.view)
        self.up = up
        self.viewDistance = viewDistance

    def __str__(self):
        return f'Camera: position={self.position}, view={self.view}, self.up= {self.up}, view distance= {self.viewDistance}'

class Ray:
    def __init__(self, origin, direction:numpy.array):
        self.origin = origin
        if(numpy.linalg.norm(direction) == 1):
            self.direction = direction
        else:
            self.direction = direction / numpy.linalg.norm(direction)

    def __str__(self):
        return f'Ray: origin={self.origin}, direction={self.direction}'

def generateRay(camera: Camera, height, width, i, j, vr, hr):
    pc = numpy.multiply(camera.viewDistance, camera.view)
    ihat = numpy.cross(camera.up, -camera.view)
    jhat = camera.up

    pi = ((i+0.5*vr/hr)/hr) - 0.5 
    pj = ((j+0.5*hr/vr)/vr) - 0.5

    ihat = numpy.multiply(ihat, width) * width/height
    jhat = numpy.multiply(jhat, height) * height/width

    pij = pc + numpy.multiply(pi, ihat) + numpy.multiply(pj, jhat)
    direction=(pij - camera.position)
    direction = direction / numpy.linalg.norm(direction)
    return Ray(camera.position, direction)

