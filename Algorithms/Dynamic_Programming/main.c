#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uintmax_t memo[255][255];

uintmax_t combM(int n, int k)
{
   if(k == 0 || n == k)
      return 1; 

   if(memo[n][k] > 0)
       return memo[n][k];

    memo[n][k] = combM(n-1, k) + combM(n-1, k-1);

    return memo[n][k];
}

uintmax_t combMB(int n, int k)
{
   if(k == 0 || n == k)
      return 1; 


    return combM(n-1, k) + combM(n-1, k-1);

}

uintmax_t combD(int n, int k)
{
   if(k == 0 || n == k)
      return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    printf("%ju\n", combM(5000,3576));
    printf("%ju\n", combMB(5000,3576));
    exit(EXIT_SUCCESS);
}
