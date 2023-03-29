def order(x, m):
    if(x == 1):
        return 1
    for r in range(2, m):
        if(pow(x, r, m) == 1):
            return r

def main():
    m = int(input("Input modulus set (int): "))

    for x in range(1, m):
        print(str(x) + " | " + str(order(x,m)))

main()
