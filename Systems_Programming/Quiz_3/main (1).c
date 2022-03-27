/*
 *  Author: Alexander Kellough
 *  NetID: atk133
 *  Compiler: GCC
 *  Description:
 *      Generates a child that acts like an active device, waits on the child to process information and then reports on that info. Handles multiple signals from the child.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

int cnl;
int cnh;
pid_t childPID;

// Parent Exit Handler
void exitP(void)
{
    kill(childPID, SIGTERM);
    char *out = "Parent has exited successfully.\n";
    write(STDOUT_FILENO, out, strlen(out));
}

// Child Exit Handler
void exitC(void)
{
    char *out = "Child has exited successfully.\n";
    write(STDOUT_FILENO, out, strlen(out));
}

// Parent Signal Handler
void signalHandlerP(int sig)
{
    char prompt[1024] = { 0 };
    char resp[2] = { 0 };
    int status, i;
    pid_t p;

    // Uses SIGCHLD to notify the user on the current state of the child process
    if(sig == SIGCHLD) 
    {
        while((p = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
        {
            if(WIFEXITED(status))
            {
                sprintf(prompt, "Child @ pid %d is exiting\n", p);
                write(STDOUT_FILENO, prompt, strlen(prompt));
            }
            if(WIFSIGNALED(status))
            {
                sprintf(prompt, "Child @ pid %d is exiting from a signal\n", p);
                write(STDOUT_FILENO, prompt, strlen(prompt));
            }
            if(WIFSTOPPED(status))
            {
                sprintf(prompt, "Child @ pid %d has paused\n", p);
                write(STDOUT_FILENO, prompt, strlen(prompt));
            }
            if(WIFCONTINUED(status))
            {
                sprintf(prompt, "Child @ pid %d has resumed\n", p);
                write(STDOUT_FILENO, prompt, strlen(prompt));
            }
        } 

        // Exits on failure
        if(p == -1)
        {
            if (errno == ECHILD)
            {
                exit(EXIT_SUCCESS);
            }
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }

    // Asks user if they want to exit, if yes, exits the program, if no, parent and child resume execution
    if(sig == SIGINT)
    {
        sprintf(prompt, "\nDo you wish to exit (y/n): ");
        write(STDOUT_FILENO, prompt, strlen(prompt));
        if(read(STDIN_FILENO, resp, 2) == -1)
        {
                perror("read");
                exit(EXIT_FAILURE);
        }
        if(resp[0] == 'y' || resp[0] == 'Y')
        {
            exit(EXIT_SUCCESS);
        } 
        else
        {
            return;
        }
    }

    // Generates the appropriate reports based on child signals
    if(sig == SIGUSR1)  
    {
        ++cnl;
        sprintf(prompt, "\nThe child has generated %d values less than 25", cnl);
        write(STDOUT_FILENO, prompt, strlen(prompt));
    }
    if(sig == SIGUSR2)  
    {
        ++cnh;
        sprintf(prompt, "\nThe child has generated %d values greater than 75", cnh);
        write(STDOUT_FILENO, prompt, strlen(prompt));
    }
}

// Child signal handler
void signalHandlerC(int sig)
{
    if(sig == SIGINT)
    {
        sleep(15);
        return;
    }
    if(sig == SIGTERM)
    {
        exit(EXIT_SUCCESS);
    }
}

// Random Integer Generator
int randInt()
{
    int range = 101;
    double scale = (((double) rand()) / ((double) RAND_MAX + 1));
    int ret = (range * scale); 
    return ret;
}

int main()
{
    // Signal handler and time struc definitions.
    struct sigaction saP;
    struct sigaction saC;
    struct timespec t;

    // Parent signal handler initialization
    saP.sa_handler = signalHandlerP;
    sigemptyset(&saP.sa_mask);
    saP.sa_flags = 0;

    // Chile signal handler initialization
    saC.sa_handler = signalHandlerC;
    sigemptyset(&saC.sa_mask);
    saC.sa_flags = 0;

    // Timespec initialization (for nanosleep)
    t.tv_sec = 15;

    // Fork switch
    switch(childPID = fork())
    {
    // If fork fails
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    
    // If in child
    case 0:
        // Exit handler assignment
        atexit(exitC);

        // Adds SIGINT to the mask
        sigaddset(&saC.sa_mask, SIGINT);

        // Associates signal handler with signals
        if(sigaction(SIGTERM, &saC, NULL) == -1)
        {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
        if(sigaction(SIGINT, &saC, NULL) == -1)
        {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }

        // Seeds rand
        srand(time(NULL));

        // Holds random value
        int val; 

        // Gets parent ID, sends appropriate signals based on random value.
        pid_t parentPID = getppid();
        while(1)
        {
            val = randInt();
            if(val < 25)
            {
               kill(parentPID, SIGUSR1); 
            }  
            if(val > 75)
            {
               kill(parentPID, SIGUSR2); 
            }
            if(48 < val && val < 51)
            {
                exit(EXIT_SUCCESS);
            }

            // Sleeps for 15 seconds
            nanosleep(&t, NULL);
        }       
    }

    // Assigns exit handler
    atexit(exitP);

    // Assocaites parent signal handler with signals
    if(sigaction(SIGCHLD, &saP, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    } 
    if(sigaction(SIGINT, &saP, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
     if(sigaction(SIGUSR1, &saP, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
     if(sigaction(SIGUSR2, &saP, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Puts program in an infinite loop until the user exits or the tasks are complete
    while(1)
    {
        continue;
    }
}
