#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* T. Ritter 2/19/2014 */

char *prompt="myshell> ";

main()
{
  int pid;
  char line[257];
  char *cmd;

    fprintf(stderr,"%s",prompt);
    while ( fgets(line,256,stdin) != NULL )
    {
    cmd = strtok(line," \t\n");   /* single word commands.... */
    switch(pid=fork())
    {
      case 0:  execlp(cmd,cmd, (char *) NULL);   /* child */
               fprintf(stderr,"ERROR %s no such program\n",line);
               exit(1);
               break;
      case -1: fprintf(stderr,"ERROR can't create child process!\n");
               break;
      default: wait(NULL);   /* parent  -- this is too simple, must wait for all children */
               break;
     }
    fprintf(stderr,"%s",prompt);
    }
    exit(0);
} /* end main */

