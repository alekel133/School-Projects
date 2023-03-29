import sys
import time

memo = []

dp = []

memoI = False

dpI = False

def init_memo(n,k):
    global memoI, memo
    if(memoI == True):
        return

    memo = [[-1 for i in range(k)] for j in range(n)]
    memoI = True

def init_dp(n,k):
    global dpI, dp
    if(dpI == True):
        return

    dp = [[0 for i in range(k)] for j in range(n)]

    dpI = True

def memo_C(n,k):
    global memo
    init_memo(n,k)
    if(n == 0 or k == 0):
        return 1

    if(n == k):
        return 1

    val = memo[n-1][k-1]

    if(val > 0):
        return val
    
    val = memo_C(n-1, k) + memo_C(n-1, k-1)
    memo[n-1][k-1] = val
    return val

def dp_C(n,k):
    global dp
    init_dp(n+1,k+1)
    for x in range(n+1):
        for y in range(k+1):
            if(y == 0 or y == x):
                dp[x][y] = 1
            
            elif (y < x):
                dp[x][y] = dp[x-1][y] + dp[x-1][y-1]

    return dp[n][k]

def C(n,k):
    if(k == 0 or n == 0):
        return 1;
    if(k == n):
        return 1
    else:
        return C(n-1, k) + C(n-1, k-1)

def output(params, values, times):
    print("|    (N,K)    |    C(N,K)    |    Time    |")
    for x in range(len(values)):
        print(params[x], end = "   |   ")
        print(values[x], end = "   |   ")
        print(times[x], end = "   |   ") 
        print()

def main():
    global memoI
    global dpI

    switch = False

    # Adjusting python recursion depth for large comparison. Note to User: Please adjust this value based on your system specs/requirements. Depending on your system, testing values that approach this recusion depth will crash python.
    # As a rule of thumb, the program recurses n times, where in is the max given by the command line argument.
    sys.setrecursionlimit(10000)

    #Get max n value from command line
    if(len(sys.argv) != 2):
        print("Invalid Syntax: main.py <int>")
        quit()

    m = int(sys.argv[1])

    # Memoization values (n, k, C(n,k), time up to max = sys.argv[1] (command line argument)
    mem_params = []
    mem_vals = []
    mem_time = []

    # Iterative Dynamic Programming values (n, k, C(n,k), time up to time = sys.argv[1] (command line argument)
    dp_params = []
    dp_vals = []
    dp_time = []


    # Recursice Combination values (n, k, C(n,k), time up to time = sys.argv[1] (command line argument)
    c_params = []
    c_vals = []
    c_time = []
            
    print("Memoization: ")
    for n in range(m+1):
        memoI = False
        k = int((n/2))
        mem_params.append([n,k]) 
        start = float(time.time())
        mem_vals.append(memo_C(n,k))
        end = float(time.time())
        stop = end-start
        mem_time.append(stop)

    output(mem_params, mem_vals, mem_time)
    print()
    print()

    print("Dynamic Programming")
    for n in range(m+1):
        dpI = False
        k = int((n/2))
        dp_params.append([n,k]) 
        start = float(time.time())
        dp_vals.append(dp_C(n,k))
        end = float(time.time())
        stop = end-start
        dp_time.append(stop)

    output(dp_params, dp_vals, dp_time)
    print()
    print()

    print("Standard Recursion (stops when case takes more than a second)")
    for n in range(m+1):
        k = int((n/2))
        if(not switch):
            c_params.append([n,k]) 
            start = float(time.time())
            c_vals.append(C(n,k))
            end = float(time.time())
            stop = end-start
            if(stop > 60):
                switch = True

            c_time.append(stop)

        else:
            c_params.append([n,k]) 
            c_vals.append("DNF") 
            c_time.append("DNF") 

    output(c_params, c_vals, c_time)

main()
