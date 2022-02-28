/*
 * Author: Alexander Kellough
 * NetID: atk133
 * Compiler: GCC
 * Description:
 *     Program emulates the behavior of the wc command.
 */
#include "hdrs.h"

int main(int argc, char *argv[])
{
    int rw, rc, i, j, bval;    
    int file = 0;
    int isSpace = 0;
    int fd = 0; 
    int wc = 0;
    int cc = 0;
    char filename[100];
    char buf = ' ';
 

    // Scans in command line arguments and sets flags.
    for(i = 0; i < argc - 1; ++i) 
    {
        if(argv[i][0] == '-')
        {
            for(j = 0; j < 3; ++j)
            {
                if(argv[i][j] == 'c')
                    rc = 1;
                if(argv[i][j] == 'w')
                    rw = 1;
            } 
        }
        else
        {
            sprintf(filename, "%s", argv[1]);
            file = 1; 
        }
    }

    // Sets default behavior if no flags are given.
    if(rw == 0 && rc == 0)
    {
        rw = 1;
        rc = 1;
    }

    // If a file is give, opens it.
    if(file)
    {
        fd = open(filename, O_RDONLY);
    }

    // Checks open system call
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Main loop
    while(1)
    {
        // Reads bytes one at a time.
        bval = read(fd, &buf, 1);
        
        // Checks the read system call.
        if(bval == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // If no info is read
        if(bval == 0)            
        {
            // If previos read wasn't a space, increase word count.
            if(!isSpace)
                ++wc;
            break;
        }

        // If the character is not a space.
        if(buf != ' ')
        {
            // If last char was a space, set isSpace to 0 and increment word count
            if(isSpace)
            {
                isSpace = 0;
                ++wc;
            }
        }

        // If read returns a space, set space flag.
        if(buf == ' ') 
        {
            isSpace = 1;
        }
        ++cc;
    }

    /* Output, close, and exit. */
    printf("    ");

    if(rw == 1)
    {
        printf("%d ", wc);
    }

    if(rc == 1)
    {
        printf("%d ", cc); 
    }

    printf(" %s\n", filename);

    close(fd);
    exit(EXIT_SUCCESS);
}
