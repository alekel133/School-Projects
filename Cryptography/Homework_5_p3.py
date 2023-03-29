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
    
def main():
    y = int(input("Enter y value: "))
    e = int(input("Enter x exponent: "))
    p = int(input("Enter modulus(if prime) or totient of modulus: "))
    m = int(input("Enter modulus if not prime: ") or "0")
    print("x = " + str(modexp(y,e,p, m)))

main()
