// Problem 1: Include Files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

pid_t childPID;

void checkCall(int val, char *str)
{
    if(val == -1)
    {
        perror(str);
        exit(EXIT_FAILURE);
    }
}    

void exitP()
{
    kill(childPID, SIGTERM);
}

void sigHandlerC(int sig)
{
    if(sig == SIGTERM)
    {
        exit(EXIT_SUCCESS);
    }
}

int child(int w, int r, char *par)
{
    checkCall(close(r), "close");
    checkCall(dup2(w, STDOUT_FILENO), "dup2");
    checkCall(execle("/bin/cat", "/bin/cat", par, NULL), "exec");
    checkCall(close(w), "close");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    int fd[2], status;
    char par[1024];
    char in[1];
    struct sigaction sa;
    sa.sa_handler = sigHandlerC;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(argc == 1)
    {
        perror("Syntax <main> <filename>");
        exit(EXIT_FAILURE);
    }

    strcpy(par, argv[1]); 

    checkCall(pipe(fd), "pipe"); 

    switch(fork())
    {
        case(-1):
            perror("fork");
            exit(EXIT_FAILURE);

        case(0):
            checkCall(sigaddset(&sa.sa_mask, SIGTERM), "sigaddset");
            checkCall(sigaction(SIGTERM, &sa, NULL), "sigaction");
            child(fd[1],fd[0], par);
    } 
    checkCall(close(fd[1]), "close");

    atexit(exitP);
    
    checkCall(wait(&status), "wait");

    while(read(fd[0], in, 1))
    {
        checkCall(write(STDOUT_FILENO, in, 1), "write"); 
    }

    checkCall(close(fd[0]), "close");

    exit(EXIT_SUCCESS);
}
