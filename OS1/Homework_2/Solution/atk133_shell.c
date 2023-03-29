/*
     Author: Alexander Kellough
     NetID: atk133
     Compoiler: gcc (GCC) 12.2.1 20230201
     Description:
          Main driver for a small unix shell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include "Command_Vector.h"

// Global variables
int pid = 0;
char history [10][256];
int flush = 0;

// Function for checking system call errors
void checkError(int err, char *str)
{
     if(err == -1)
     {
         perror(str);
         exit(EXIT_FAILURE);
     }
}

// Signal Handler
void sigHandler(int sig)
{
     if(sig == SIGINT)
     {
          if(pid != 0)
               kill(pid, SIGINT);

          return;
     }
     if(sig == SIGUSR1)
     {
          flush = 1;
     }
}

// Child Function 
// Note to Grader: I could not get redirection to work properly, I would be very appreciative if you could email me a solution
// at atk133@msstate.edu. It has reall;y been bothering me.
void child(struct commandVector com)
{
     struct commandVector com1;
     struct commandVector tmp;
     int cpid;
     int recheck = find(&com, ">");


     /*
     if(recheck)
     {
          switch(cpid = fork())
               {
                    case -1: 
                    fprintf(stderr,"ERROR cant create child process");
                    break;

                    case 0:
                         recheck = 0;
                         child(com1);
                         break;
                    
                    default:
                         wait(NULL);
               }
     }
     */ 

     // Execute command described in com, print error if the process doesn't exit.
     execvp(com.arr[0], com.arr);
     fprintf(stderr,"ERROR %s: no such program or file/\n", com.arr[0]);
     exit(EXIT_FAILURE);
}

int main(int argc, char *argv[], char *env[])
{
     char* disown;
     int h = 0;
     char line[256];
     char *prompt =  getcwd(NULL,0);
     struct commandVector com;
     struct sigaction sa;

     // Signal Handler Initialization
     sa.sa_handler = sigHandler;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;

     sigaddset(&sa.sa_mask, SIGINT);

     checkError(sigaction(SIGUSR1, &sa, NULL), "ERROR: sigaction");
     checkError(sigaction(SIGINT, &sa, NULL), "ERROR: sigaction");


     // Add the input arrow to prompt text (taken from pwd).
     strcat(prompt, "> ");
     fprintf(stderr,"%s",prompt);


     // Main loop
     while(fgets(line, 256, stdin) != NULL)
     {  
          // Flush bit
          flush = 0;

          // Make sure that we don't go over history arry boundary
          if(h > 9)
          {
               for(int i = 1; i < 11; ++i)
               {
                    strcpy(history[i-1], history[i]);
               }
               h = 9;
          }
          else
          {
               strcpy(history[h],line);
               ++h;
          }

          // Split command by whitespace and the & character
          disown =  strchr(line, '&');
          tokenize(&com, line, " \t\n&");

          // Check internal commands
          if(!strcmp(com.arr[0], "cd"))
          {    
               chdir(com.arr[1]);
          }

          else if(!strcmp(com.arr[0], "history"))
          {
              for(int i = 0; i < h; ++i)
              {
                    fprintf(stderr, "%s", history[i]);
              }
          }

          else if(!strcmp(com.arr[0], "env"))
          {
               int x = 0;
               while(env[x] != NULL)
               {
                    fprintf(stderr, "%s\n", env[x]);
                    ++x;
               }
          }

          else if(!strcmp(com.arr[0], "clear"))
          {
               for(int i = 0; i < 128; ++i)
               {
                    fprintf(stderr, "%s", "\n");
               }
          }

          else if(!strcmp(com.arr[0], "exit"))
          {
               deconstruct(&com);
               exit(EXIT_SUCCESS);
          }

          else if(!strcmp(com.arr[0], "flush"))
          {
               kill(getpid(), SIGUSR1); // Send USR1 to self, triggering flush bit flip
          }

          else {
          
               // If no internal commands match, spawn child process and execute program
               switch(pid = fork())
               {
                    case -1: 
                    fprintf(stderr,"ERROR cant create child process");
                    break;

                    case 0:
                         child(com);
                         break;
                    
                    default:
                         if(disown == NULL)
                              wait(NULL);
               }
          }
          
          // If flush bit is flipped, write history buffer to audit.log
          if(flush)
          {
               FILE *audit = fopen("audit.log", "w+");
               for(int i = 0; i < h; ++i)
               {
                    fprintf(audit,"%s", history[i]);
               }
               fclose(audit);
          }

          // Clear command vector, update prompt, and continue loop
          clear(&com); 
          prompt = getcwd(NULL, 0);
          strcat(prompt, "> ");
          fprintf(stderr,"%s",prompt); 
     } 

     // Exit Program, Shouldn't get here because an explicit exit should be called.
     exit(EXIT_SUCCESS);
}