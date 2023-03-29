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

def main():
    x = int(input("Input a starting power: "))
    start = 2**x
    print("Result: " + str(ps(start)))

main()
