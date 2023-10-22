import numpy

class Ray():
    def __init__(self, origin: numpy.array, direction: numpy.array):
        self.origin = origin
        self.dir = direction  

    def __str__(self):
        return f"Ray: r(t) = {self.origin} + t{self.dir}"

    # Evaluates the point corresponding to the ray t value: r(t)
    def eval(self, t: float):
        return self.origin + self.dir * t

    def transformRay(self,mat):
        origin = transform(mat, self.origin) 
        dir = normalize(transform(mat,self.dir))
        return Ray(origin, dir)


# Used to generate ray after finding specific viewpoint
def generateRay(origin: numpy.array, port: numpy.array):
    return Ray(origin, port-origin)
