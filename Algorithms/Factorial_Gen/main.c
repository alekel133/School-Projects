#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

long long buf [256];
long long i = 1;

void flush_buf() 
{
    printf("[%lld,", buf[0]);
    for(int j = 1; j  < 256; ++j)
    {
        printf(" %lld,", buf[j]);
    }
    printf(" %lld]", buf[255]);
}

void exitH() 
{
    flush_buf();
}

long long fac(long long n) 
{
    long long result = n;

    for(long long m = 1; m < n; ++m)
    {
        result *= m;    
    }

    return result;
}

int main(int argc, char *argv[])
{
    
    atexit(exitH);
    while(i < 257)
    {
       buf[i-1] = fac(i); 
       ++i;
    }

    exit(EXIT_SUCCESS);
}
