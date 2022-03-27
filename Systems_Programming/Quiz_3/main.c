#include "hdrs.h"
#include <sys/wait.h>

int main()
{
    int num_child;
    int num_term = 0;
    pid_t childPID;

    printf("Enter the amount of children to spawn: ");
    scanf("%d", &num_child);
    printf("\n");
    
   while(num_term < num_child)
    {  
    switch(childPID = fork()) 
    {
    case -1:
        exit(EXIT_FAILURE);
    case 0:
        printf("Child spawned.\n");
        exit(EXIT_SUCCESS); 
    default:
        while(1)
        {
        childPID = wait(NULL);
        if(childPID == -1)
        {
            if(errno == ECHILD)
            {
                printf("All done.\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                perror("while");
                exit(EXIT_FAILURE);
            }
        }
        }
    }
    ++num_term;
    }
}
