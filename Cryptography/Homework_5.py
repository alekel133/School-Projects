import random as rand

def ps(x):
    start = x
    while(pc(x,100) == 0):
        x = x+1
    print(x-start)
    return x

def pc(x, mct):
    if x%2 == 0:
        return 0
    ct = 0
    while ct < mct:
        a = rand.randint(2, x-2)
        if pow(a, x-1, x) == 1:
            ct = ct+1
        else:
            return 0
        print(ct)
    return x

def qrt(x, p):
    u = int((p-1)/2)
    y = pow(x, u, p)

    if(y == 1):
        return 1

    return 0

def modsqroot(x, p):
    v = int((p+1)/4)
    if(p % 4 != 3):
        return None
    if(qrt(x,p) == 0):
        return None

    return pow(x, v, p)

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
    return None

def modexp(y,n,p,m):
    d = multinv(n,p-1)
    if(d == None):
        return modexpg(y,n,p,m)
    return pow(y,d,p)

def modexpg(y,n,tp, m):
    d = multinv(n, tp)
    return pow(y,d,m)

def order(x, m):
    if(x == 1):
        return 1
    for r in range(2, m):
        if(pow(x, r, m) == 1):
            return r

def main():
    x = int(input("Input a starting power: "))
    start = 2**x
    print("Result: " + str(ps(start)))
    x = int(input("Enter the oprand value: "))
    p = int(input("Enter the modulus value: "))
    print("The square root is: " + str(modsqroot(x, p)))
    y = int(input("Enter y value: "))
    e = int(input("Enter x exponent: "))
    p = int(input("Enter modulus(if prime) or totient of modulus: "))
    m = int(input("Enter modulus if not prime: ") or "0")
    print("x = " + str(modexp(y,e,p, m)))
    m = int(input("Input modulus set (int): "))
    for x in range(1, m):
        print(str(x) + " | " + str(order(x,m)))

main()
