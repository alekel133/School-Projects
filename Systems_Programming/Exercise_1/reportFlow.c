
#include "hdrs.h"

union ISTR
{
    char bytes[4];
    int val;  
};

int reportFlow(int val)
{
    if(val < 550)
    {
        printf("Line pressure low. "); 
        return -1;
    }
    if(val > 1450)
    {
        printf("Line pressure high. ");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int i = 0;
    int reset;
    char filename[100] = "flowdata.dat";
    int fd;
    union ISTR in;

    if(argc != 1 && argc != 2)
    {
        perror("Syntax reportFlow <filename (optional)>\n");
        exit(EXIT_FAILURE);
    }
    
    if(argc == 2)
    {
        sscanf(argv[1], "%s", filename);
    }

    fd = open (filename, O_RDONLY); 

    while(read(fd, in.bytes, 2))
    {
        reset = reportFlow(in.val);
        ++i;
        if(reset == -1)
        {
            printf("%d readings since last report.\n", i);
            i = 0;
        } 
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
