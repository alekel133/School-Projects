def egcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m


def main():
    p = int(input("Enter p: "))
    g = int(input("Enter g: "))
    a = int(input("Enter a: "))

    alpha = pow(g,a,p)

    print(alpha)

    m = int(input("Input m: "))
    k = int(input ("Input k: "))

    ki = modinv(k, p-1)
    print(ki)

    r = pow(g,k,p)
    s = ((m - a*r)*ki)%(p-1)

    print(r)
    print(s)

main()
