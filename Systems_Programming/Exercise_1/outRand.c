/*
    Aurhor: Alexander Kellough
    MSU Student ID: atk133
    Compiler: GCC
    Description:
        Genarates random numbers/offsets and writes them to file to imitate device input on a unix operatin system.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

// Union to remove unused bytes from integer before writing to file.
union ISTR
{
    int val;
    char bytes[4];
};

// Generates random integer and offset; returns offset average from ranges.
int randInt(int low, int middle, int high)
{             
    int range = 101;
    double scale = (((double) rand()) / ((double) RAND_MAX + 1)); 
    double offset_scale =  (((double) rand()) / ((double) RAND_MAX + 1));
    int sel = scale  * range;
    double offset = ((offset_scale) - 0.5);

    if(sel < 16)
    {
        return (low + offset) * 100;
    }     
    if(sel > 84)
    {
        return (high + offset) * 100;
    }
    return (middle + offset) * 100;
}                                                                          

int main(int argc, char *argv[])
{
    int i = 0;
    int number = 60;
    int low = 5, middle = 10, high = 15;
    char filename[100] = "flowdata.dat";
    int val;
    int fd;
    int outBytes;
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
    union ISTR out;

    // Seeds random function
    srand(time(NULL));

    // Checks command line argument syntax
    if(argc != 1 && argc != 3 && argc != 6)
    {
         printf("sntax: outRand <filename> <number> or outRand <filename> <number> <low> <middle> <high>\n");
         exit(EXIT_SUCCESS);
    }

    // Converts comand line arguments to appropriate types.
    if(argc == 3)
    {
        sscanf(argv[1], "%s", filename);    
        sscanf(argv[2], "%d", &number);
    }

    if(argc == 6)
    {
        sscanf(argv[1], "%s", filename);    
        sscanf(argv[2], "%d", &number);
        sscanf(argv[3], "%d", &low);    
        sscanf(argv[4], "%d", &middle);
        sscanf(argv[5], "%d", &high);     
    }

    // Opens file
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, perms);
    
    // Generates random numbers using randInt and writesthem to file.
    for(i = 0; i < number; i++)
    {
        val = randInt(low, middle, high); 
        out.val = val;
        printf("%d\n", val);  
        outBytes = write(fd, out.bytes, 2);
        if(outBytes == -1)
        {
           perror("Writing File");
           close(fd);
           exit(EXIT_FAILURE);
        }
    }

    // Closes file.
    close(fd);
    
    exit(EXIT_SUCCESS);
}
