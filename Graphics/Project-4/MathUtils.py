import numpy

def degToRad(theta):
    return theta *numpy.pi/180

def radToDeg(theta):
    return theta *180/numpy.pi

def normalize(array: numpy.array):
    return array / numpy.linalg.norm(array)

def transform(vector: numpy.array, mat: numpy.array):
    vec = numpy.dot(mat, vector)
    return vec

def rotateX(theta, mat):
    sin = numpy.sin(theta)
    cos = numpy.cos(theta)
    rot = numpy.array(([1, 0, 0, 0],
                 [0, cos, -sin, 0],
                 [0, sin, cos, 0],
                 [0, 0, 0, 1]))

    return transform(mat, rot)

def rotateY(theta, mat):
    sin = numpy.sin(theta)
    cos = numpy.cos(theta)
    rot = numpy.array(([cos, 0, sin, 0],
                       [0, 1, 0, 0],
                       [-sin, 0, cos, 0],
                       [0, 0, 0, 1]))

    return transform(mat, rot)

def rotateZ(theta, mat):
    sin = numpy.sin(theta)
    cos = numpy.cos(theta)
    rot = numpy.array(([cos, -sin, 0, 0],
                       [sin, cos, 0, 0],
                       [0, 0, 1, 0],
                       [0, 0, 0, 1]))

    return transform(mat, rot)

def translate(x, y, z, mat):
    tra = numpy.array(([1, 0, 0, -x],
                       [0, 1, 0, -y],
                       [0, 0, 1, -z],
                       [0, 0, 0, 1]))

    return transform(mat, tra)