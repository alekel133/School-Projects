import numpy

class Camera:
    def __init__(self, position:numpy.array, up:numpy.array, view: numpy.array, viewDistance: float):
        self.position = position
        self.up = up/numpy.linalg.norm(up)
        self.view = view/numpy.linalg.norm(view)
        self.viewDistance = viewDistance

    def __str__(self):
        return f'Camera: position={self.position}, up={self.up}, view={self.view}, view distance= {self.viewDistance}'

class Ray:
    def __init__(self, origin, direction:numpy.array):
        self.origin = origin
        if(numpy.linalg.norm(direction) == 1):
            self.direction = direction
        else:
            self.direction = direction / numpy.linalg.norm(direction)

    def __str__(self):
        return f'Ray: origin={self.origin}, direction={self.direction}'

def generateRay(camera: Camera, height, width, i, j):
    pc = numpy.multiply(camera.viewDistance, camera.view)
    ihat = numpy.cross(camera.up, -camera.view)
    jhat = camera.up
    
    pi = ((i+0.5)/width) - 0.5
    pj = ((j+0.5)/height) - 0.5

    ihat = numpy.multiply(ihat, width)
    jhat = numpy.multiply(jhat, height)

    pij = pc + numpy.multiply(pi, ihat) + numpy.multiply(pj, jhat)

    direction  = (pij - camera.position)

    return Ray(camera.position, direction)

