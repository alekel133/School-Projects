def main():
    p = int(input("Enter public p: "))
    g = int(input("Enter public g: "))
    a = int(input("Enter random secret a: "))
    b = int(input("Enter random secret b: "))

    alpha = pow(g, a, p)
    beta = pow(g, b, p)

    print("Alpha: " + str(alpha))
    print("Beta: " + str(beta))

    Ka = pow(alpha, b, p)
    Kb = pow(beta, a, p)

    if(Ka != Kb):
        print("Something went wrong!")

    else:
        print(Ka)

main()
