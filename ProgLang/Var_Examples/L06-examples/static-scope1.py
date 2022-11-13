# static scoping example
# python3 static-scope1.py
x = 5
y = 3

def sub1():
    x = 7
    print("sub1 x:", x)

    def sub2():
        x = 9
        print("sub2 x:", x)

    sub2()
    y = 10

sub1()
print("base x:", x)
print("base y:", y)
