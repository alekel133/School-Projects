#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*   Build a simple command vector and overwrite my process with the new command.
 *        TR
 *        */

main(argc,argv)
int argc;
char *argv[];
{

    char **mycmd;

    mycmd=(char **) malloc( 2 * sizeof(char *));
    mycmd[0]=(char *) malloc( 10 * sizeof(char));
    mycmd[1]=NULL;

    strcpy(mycmd[0],"who");  /* put a command in the vector */
 
    int fileout=open("my-output.txt",O_WRONLY|O_TRUNC|O_CREAT,0644);
    dup2(fileout,1);  /* stdout goes to fileout */
    close(fileout);   /* don't need fileout, the dup is fine */

    execvp(mycmd[0],mycmd);   /* first argument is executeable name, 2nd is vector including name and any args */

    printf("It did not work if this prints!\n");
    exit(1);
}
