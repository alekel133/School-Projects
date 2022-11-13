# static scoping example
# python3 static-scope2.py
x = 5
y = 3
# Printed in the form variable: (l-value) = (r-value)
#print(f"base x: ({id(x)}) = {x}")
#print(f"base y: ({id(y)}) = {y}")
def sub1():
    #global x
    #print(f"\tsub1 x: ({id(x)}) = {x}") # ERROR: Reference before assignment
    #print("\tassign x in sub1")
    x = 7
    print(f"\tsub1 x: ({id(x)}) = {x}")

    def sub2():
        #global y
        #print("\t\tassign x in sub2")
        x = 9
        print(f"\t\tsub2 x: ({id(x)}) = {x}")
        #print(f"\t\tsub2 y: ({id(x)}) = {y}") # ERROR: Reference before assignment
        #print("\t\tsub2 returned")

    #print(f"\tsub1 x: ({id(x)}) = {x}")
    sub2()
    #print(f"\tsub1 x: ({id(x)} = {x})")
    #print("\tassign y in sub1")
    #global y
    #print(f"\tsub1 y: ({id(y)}) = {y}") # ERROR: Reference before assignment
    y = 10
    #print(f"\tsub1 y: ({id(y)}) = {y}")

sub1()
print(f"base x: ({id(x)}) = {x}")
print(f"base y: ({id(y)}) = {y}")
