/*      
 *      Author: Alexander Kellough
 *      MSU NetID: atk133
 *      Compiler: GCC
 *      Description:
 *          Reads in data from a specified file (defailt flowdata.dat) and reports on the flow represented by the data on that file.
 */
#include "hdrs.h"

// Union for converting 2bit values to 4 bit integers.
union ISTR
{
    char bytes[4];
    int val;  
};

// Checks data, and reports it's flow if necessary. Returns -1 or 0 based on if a report was made or not.
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

    if(argc != 1 && argc != 2) // Checks if command line arguments match syntax.
    {
        perror("Syntax reportFlow <filename (optional)>\n");
        exit(EXIT_FAILURE);
    }
    
    if(argc == 2) // If optional command line argument is given, assigns it to filename.
    {
        sscanf(argv[1], "%s", filename);
    }

    fd = open (filename, O_RDONLY); 

    if(fd == -1)
    {
        perror("open(filename)");
        exit(EXIT_FAILURE);
    }

    while(read(fd, in.bytes, 2)) // Reads in 2 bytes of data from input file.
    {
        reset = reportFlow(in.val); // Sends data to reportFlow(int val), for analysis and reporting.
                                    
        ++i; // Keeps count of data memebers read since last report.

        if(reset == -1) // If a report was printed, prints readings since last report and resets count.
        {
            printf("%d readings since last report.\n", i);
            i = 0;
        } 
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
