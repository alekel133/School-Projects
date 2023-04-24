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
    p = int(input("Input Public p: "))
    g = int(input("Input Public g: "))
    a = int(input("Input a: "))

    alpha = pow(g,a,p)
    print(alpha)

    k = int(input("Input mask: "))
    P = int(input("Input Plaintext: "))

    mu = pow(g, k, p)
    ak = pow(alpha, k, p)

    print(mu)
    print(ak)

    c = (P * ak) % p

    print(c)

main()
