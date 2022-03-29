#include "hdrs.h"

union STRI
{
    char bytes[4];
    int val;
};

int main()
{
    int status, fd, filenum, ret, sum, average;
    char filename[12];
    union STRI in;
    
    switch(fork()) 
    {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        ret = execl("./myRand.out", "./myRand.out", NULL);
        if(ret == -1)
        {
            perror("exec");
            exit(EXIT_FAILURE);
        }
    }
    wait(&status);
    if(WIFEXITED(status))
    {
       filenum = WEXITSTATUS(status); 
    }
    
    sprintf(filename, "data%d.dat", filenum);
    fd = open(filename, O_RDONLY);
    
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 60; ++i)
    {
        ret = read(fd, in.bytes, 4); 
        if(ret == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        printf("%d\n", in.val);
        sum += in.val;
    }
    average = sum/60;

    printf("Average is: %d\n", average);
    close(fd);
    exit(EXIT_SUCCESS);
}
