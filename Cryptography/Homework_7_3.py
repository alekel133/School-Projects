def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        return None
    else:
        return x % m

def main():
    p = int(input("Input p: "))
    q = int(input("Input q: "))
    n = p * q
    phin = (p-1)*(q-1)

    g = 0
    while(g != 1):
        e = int(input("Try a value of e: "))
        g,t1, t2 = egcd(e, phin)

    print("That Works!")

    d = modinv(e, phin)
    if d == None:
        raise Exception("Modular inverse did not exist")

    print(n, e)
    print(d)

    P = int(input("Input Plaintext: "))
    C = pow(P, e, n)

    print(C)

    m = int(input("Input Hash: "))
    s = pow(m, d, n)

    print(s)

main()
