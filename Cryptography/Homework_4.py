"""
    Author: Alexander Kellough
    NetId: atk133
    Description:
        File for computing necassary values for use in Assignment 4 of Cryptography.
"""


### These functions are taken from the lecture notes on Number Theory
def gcd(a,b):
    while v:
        u,v = u, u%v
    return u

def xgcd(u,v):
    preva,a=u,v 
    prevs, s = 1, 0; prevt, t = 0, 1
    while a:
        q=preva//a
        s, prevs = prevs-q*s, s
        t, prevt = prevt-q*t, t
        a, preva = preva-q*a, a
    return preva, prevs, prevt

def multinv(a,m):
    g, x, y = xgcd(a,m)
    if g == 1:
        return x % m


### Formated output for Assignment 4
def main():
    print("1 a) " + str(multinv(234,547)) + "\n  b) " + str(multinv(233,546721)) + "\n  c) " +
                        str(multinv(273, 54672)) + "\n")

    
    a,s,t = xgcd(54678,10481)

    print("2) " + str(a) + " = " +str(s) + "*54678 + " +str(t) + ("*10481"))

    y = multinv(1626364, 2345699)
    z = (234 - 2353545) % 2345699
    w = (y*z) % 2345699
    print("3) x = " + str(y) + " * " + str(z) + " = " + str(w))

    print("4) " +str((multinv(238,547) * 77)%547))

main()
