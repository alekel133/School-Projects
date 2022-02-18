#include "hdrs.h"

int main(int argc, char *argv[])
{
    int rw, rc, i, j, isSpace, bval;    
    int fd = STDIN_FILENO; 
    int wc = 0;
    int cc = 0;
    char filename[100] = " ";
    char *buf;
 

    for(i = 0; i < argc; ++i) 
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
            strcpy(filename, argv[i]);
    }

    if(rw == 0 && rc == 0)
    {
        rw = 1;
        rc = 1;
    }

    while(1)
    {
        bval = read(fd, buf, 1);
        
        if(bval != 1)
            break;

        if(*buf == EOF)
            break;

        if(*buf == ' ' || *buf == '\n')
        {
            isSpace = 1;
        }

        if(isSpace != 1 && *buf != ' ' && *buf != '\n')
        {
            ++wc;
            isSpace = 0;
        }

        ++cc;
    }

    if(rc == 1)
    {
        printf("    %d", cc);
    }

    if(rw == 1)
    {
        printf("    %d", wc); 
    }

    printf("    ");
    exit(EXIT_SUCCESS);
}
