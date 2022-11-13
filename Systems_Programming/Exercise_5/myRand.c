/*  Author: Alexander Kellough
 *  NetID: atk133
 *  Compiler: GCC
 *  Description:
 *      Generate a random file name and store 60 random numbers(0-100) in that file.
 */
#include "hdrs.h"
#include <time.h>

// Union for integer string conversion
union ISTR
{
    int val;
    char bytes[2];
};

// Gernerate random integer in range(0, int range);
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

    // Seed random
    srand(time(NULL));
    
    // Generate filename and store it
    filenum = randInt(255);
    sprintf(filename, "data%d.dat", filenum);
    
    // open file
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, perms);

    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Generate and write 60 random numbers to file
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

    // Close file and exit
    close(fd);
    exit(filenum);
}
