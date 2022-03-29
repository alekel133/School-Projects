#include "hdrs.h"
#include <time.h>

union ISTR
{
    int val;
    char bytes[2];
};

int randInt(int range)
{
    double scale = (((double) rand()) / ((double) RAND_MAX + 1));
    return scale * range;
}

int main()
{   
    int filenum,fd;
    char filename[12];
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    union ISTR out;

    srand(time(NULL));
    
    filenum = randInt(255);
    sprintf(filename, "data%d.dat", filenum);
    
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, perms);

    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 60; ++i)
    {
        out.val = randInt(100);
        int ret = write(fd, out.bytes, 4 ); 
        if(ret == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    } 

    close(fd);
    exit(filenum);
}
