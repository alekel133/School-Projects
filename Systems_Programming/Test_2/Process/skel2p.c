/* 
 *  Author: Alexander Kellough
 *  NetID: atk133
 *  Compiler: GCC
 *
 *
 * Write a multiple concurrent process program that does the following
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into 
      a file named data.dat
   3. Determines how many integers are > 100
   4. Determines how many integers are < 100
   5. Outputs the total count for each group.

   The program should perform this task in the following way:
   Create a producer child that:
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into a file
      named data.dat
   Create a consumer child that:
1.	For each value in the file
a.	Determine if value > 100
b.	If the value > 100, signals the parent with SIGUSR1
   Create a consumer child that:
1.	For each value in the file
a.	Determines if value < 100
b.	If the value < 100, signals the parent with SIGUSR2
   Create a parent that: 
   1. Creates children 1 - 3
   2. Pauses. 

 */

/* Include files */
#include <stdio.h>
#include <stdlib.h>

/* Problem 1. Add the necessary include files for this program */
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>


/* 
   Global variables:
   For this project I am storing the pid's of the three children
   and two integers that serve the role of flags and counters

 */

/* Problem 2. Declare the global variables for pid's of the three
   children and the two integers that serve the role of flags and
   counters. The first flag deals with the large count; the second
   flag deals with the small count.
 */

#define BUF_SIZE 1024
pid_t children[3];
int large = 0;
int small = 0;

void exitP()
{
    for(int i = 0; i < 3; ++i) 
    {
        if(kill(i, SIGTERM) == -1)     
        {
            continue;
        }
    }
}
/* myPrint is a convenience function to use when we are in a signal 
   handler. This is because printf uses buffered I/O.
 */
void myPrint(const char *str)
{
  if (write(STDOUT_FILENO, str, strlen(str)) == -1)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }
}

/* Signal handlers
   We will need to implement at least two signal handlers. 
   One for the parent and a minimum of one for the children
 */

/* Problem 3. Define and write the signal handler for the parent */
void signalHandlerP(int sig)
{
    if(sig == SIGUSR1)
    {
        ++large;
    }
    if(sig == SIGUSR2)
    {
        ++small;
    }
    if(sig == SIGCHLD)
    {
        char *l_out = (char*) malloc(sizeof(char*));
        char *s_out = (char*) malloc(sizeof(char*));

        kill(children[1], SIGUSR1);
        kill(children[2], SIGUSR2);
        
        wait(NULL);
        wait(NULL);

        sprintf(l_out ,"Larger: %d\n", large); 
        sprintf(s_out ,"Smaller: %d\n", small);
        
        myPrint(l_out); 
        myPrint(s_out);

        free(l_out);
        free(s_out);
        exit(EXIT_SUCCESS);
    }
}

/* Problem 4. Define and write the signal handler for the children */
void signalHandlerC(int sig)
{
    if(sig == SIGUSR1)
        large = 1;
    if(sig == SIGUSR2)
        small = 1;
}

/* Functions for each of the children. 
   We will be writing functions for each of the three children.
   This should make it easier to answer the questions on threads.
 */

/* Problem 5. Define and write the function for child 1. */
void child1()
{
    int fd, tot, val, ret;


    printf("How many integers would you like to enter: ");
    scanf("%d", &tot); 
    printf("\n");

    fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < tot; ++i)
    {
        printf("Integer %d: ", i+1);
        scanf("%d", &val);
        printf("\n");
        ret = write(fd, &val, 4); 
        if(ret == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    printf("Thank You\n");

    close(fd);
    exit(EXIT_SUCCESS);
}

/* Problem 6. Define and write the function for child 2. */
void child2()
{
    int fd, val;
    int count = 0;
    pid_t ppid = getppid();
    struct sigaction sa;

    sa.sa_handler = signalHandlerC;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    } 

    while(large == 0) 
    {
        continue;
    }

    fd = open("data.dat", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while(read(fd, &val, 4))
    {
        if(val > 100)
        {
            ++count;
            kill(ppid, SIGUSR1);
        }
    } 

    close(fd);
    printf("Larger: %d\n", count);
    exit(EXIT_SUCCESS);
}

/* Problem 7. Define and write the function for child 3. */
void child3()
{
    int fd, val;
    int count = 0;
    pid_t ppid = getppid();
    struct sigaction sa;

    sa.sa_handler = signalHandlerC;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    } 

    while(small == 0) 
    {
        continue;
    }

    fd = open("data.dat", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while(read(fd, &val, 4))
    {
        if(val < 100)
        {
            ++count;
            kill(ppid, SIGUSR2);
        }
    } 

    close(fd);
    printf("Smaller: %d\n", count);
    exit(EXIT_SUCCESS);

}





/* This function forks a child and runs the function passed
   in after the child has successfully forked. I have provided
   it to make the code easier to read.
 */
pid_t hndlFork(void (*child)(void))
{
  pid_t p;
  p = fork();
  if (p == -1)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }
  if (p == 0)
    {
      child();
    }
  return p;
}

/* Problem 8: Define and write the function main */


int main(int argc, char *argv[])
{
    struct sigaction sa;  

    sa.sa_handler = signalHandlerP;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    children[1] = hndlFork(child2);     
    children[2] = hndlFork(child3);

    if(sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGUSR2, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_SUCCESS);
    }

    children[0] = hndlFork(child1);

    if(sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        continue;
    }
    exit(EXIT_SUCCESS);
}
