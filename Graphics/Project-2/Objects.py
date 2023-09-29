import Ray
import numpy

class Object:
    def __str__(self):
        return "Object"
    def isHit(self, ray:Ray):
        return tuple
       
class Sphere(Object):
    def __init__(self, center:numpy.array, radius:float, color:tuple):
        self.center = center
        self.radius = radius
        self.color = color

    def __str__(self):
        return f'Sphere: center={self.center}, radius={self.radius}, color={self.color}'


    def isHit(self, ray:Ray):

        # Adopted from code by Dr. Jankun-Kelly provided on canvas: "Basic Ray Casting"
        # Because direction gets normalized on initilization, a is eliminated
        b = numpy.dot(ray.direction, ray.origin - self.center)
        c = numpy.linalg.norm(ray.origin - self.center)- self.radius**2
        disc = (b**2)-4*c

        # If the discreminant is negative, we get imaginary roots, 
        # and thus the ray doesn't hit in the 3d plane
        if(disc < 0):
            return (False, None)
        
        t1 = -b + numpy.sqrt(disc)
        t0 = -b - numpy.sqrt(disc)

        if(t0 > 0 and t1 > 0):
            return (True, (min(t0, t1), self.color))

        elif t1 > 0:
            return (True, (t1, self.color))

        elif t0 > 0:
            return (True, (t0, self.color))

        return(False, None)


class Plane(Object):
    def __init__(self, origin:numpy.array, normal: numpy.array, color: tuple):
        self.origin = origin
        self.normal = normal/numpy.linalg.norm(normal)
        self.color = color

    def __str__(self):
        return f'Plane: origin={self.origin}, normal={self.normal}, color={self.color}'

    def isHit(self, ray: Ray):

        # The following code is adpated from the examples given in 
        # "Fundamentals of Computer Graphics" by Steven Marschner
        # and Peter Shirley on triangle intersection, as well as 
        # a papaer by Brian Curless of Washington University entitiled
        # "Ray-Triangle intersection"

        # Test to see if the ray is parallel to the plane
        # if not, it must intersect the plane at some point
        # So return true, the color to be written at that pixel
        # and the t value for further computation (testing for Triangles)

        pEq = numpy.dot(self.normal, self.origin)
        bottom = numpy.dot(self.normal, ray.direction)
        if bottom == 0:
            return (False, None)

        t = pEq -numpy.dot(self.normal, ray.origin) / bottom
        if(t < 0):
            return (False, None)

        return(True, (t, self.color))

class Triangle(Object):
    def __init__(self, points: list[numpy.array], color:tuple):

        # The following code is adapted from Dr. Jankun-Kelly's
        # In-class examples, The section on Ray-Triangle Intersection
        # In "Fundament of Computer Graphics" by Steven Marschener and
        # Peter Shirley, and a paper entitled "Ray-Triangle Intersection"
        # By Brian Curlessof Washington University

        self.a = points[0]
        self.b = points[1]
        self.c = points[2]

        notUnitNorm = numpy.cross(numpy.subtract(self.b,self.a), numpy.subtract(self.c,self.a))

        self.normal = notUnitNorm / numpy.linalg.norm(notUnitNorm)

        self.color = color

    def __str__(self):
        return f'Triangle: p1={self.a}, p2={self.b}, p3={self.c}, normal={self.normal}, color={self.color}'

    def isHit(self, ray:Ray):
        p = Plane(self.normal, self.a, 0)

        status = p.isHit(ray)

        # If the ray doesn't hit the plane the triangle
        # Sits on, then it can't hit the triangle
        # So stop computing
        if status[0] != True:
            return (False, None)

        ret = status[1]
        
        intersection = numpy.subtract(ray.origin, numpy.multiply(ret[0], ray.direction))

        ab = numpy.cross(numpy.subtract(self.b,self.a), numpy.subtract(intersection, self.a))
        bc = numpy.cross(numpy.subtract(self.c,self.b), numpy.subtract(intersection, self.b))
        ca = numpy.cross(numpy.subtract(self.a,self.c), numpy.subtract(intersection, self.c))

        if numpy.dot(ab, self.normal) < 0:
            return (False, None)
        if numpy.dot(bc, self.normal) < 0:
            return (False, None)
        if numpy.dot(ca, self.normal) < 0:
            return (False, None)


        return(True, (ret[0], self.color))
        
