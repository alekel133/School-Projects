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


def main():
    x = int(input("Enter the oprand value: "))
    p = int(input("Enter the modulus value: "))
    print("The square root is: " + str(modsqroot(x, p)))


main()
