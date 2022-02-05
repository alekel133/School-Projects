#include "hdrs.h"
#include "dirent.h"

char *dupStr(const char *str)
{
    char *cdup;
    int len;
    
    if(str == NULL) return NULL;

    len = strlen(str);
    cdup = (char *) calloc(len+1, sizeof(char));

    strcpy(cdup,str);
    return cdup;
}

int main(int argc, char *argv[])
{
    
    DIR * dirp;
    struct dirent * dp;
    struct stat *sp;

    char *dirname;
    char fname[1024];
    int isCurr = 0; //false
    
    dirname = (argc ==1) ? dupStr(".") : dupStr(argv[1]); 

    isCurr = !strcmp(dirname, ".");
    
    dirp = opendir(dirname);
    if(dirp == NULL)
    {
        perror("opedir");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL) break;

        if(isCurr) { sprintf(fname, "%s", dp->d_name);} 
        else {sprintf(fname,"%s/%s", dirname, dp->d_name); }

        if(stat(fname,sp) == -1)
        {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(sp->st_mode)) printf("%s\n", fname);
        
    }

    if(closedir(dirp) == -1)
    {
        perror("closedir");
        exit(EXIT_FAILURE); 
    }

    exit(EXIT_SUCCESS);
}
