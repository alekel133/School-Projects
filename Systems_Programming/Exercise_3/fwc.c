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
        {
            strcpy(filename, argv[i]);
        }
    }

    if(rw == 0 && rc == 0)
    {
        rw = 1;
        rc = 1;
    }

    if(strlen(filename) > 0)
        fd = open(filename, O_RDONLY);
    
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        bval = read(fd, &buf, 1);
        
        if(bval == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if(bval == 0)
        {
            if(!isSpace)
                ++wc;
            break;
        }

        if(buf != ' ')
        {
            if(isSpace)
            {
                isSpace = 0;
                ++wc;
            }
        }

        if(buf == ' ') 
        {
            isSpace = 1;
        }
        ++cc;
    }

    if(rw == 1)
    {
        printf("    %d", wc);
    }

    if(rc == 1)
    {
        printf("    %d", cc); 
    }

    printf("    ");

    close(fd);
    exit(EXIT_SUCCESS);
}
