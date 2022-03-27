/* Author: Alexander Kellough
 * MSU NetID: atk133
 * Compiler: GCC
 * Description:
 *      Program that uses parent and child to read and report on contents of a file and take input simultaneously.
 */

#include "hdrs.h"
#include <time.h>
#define BUF_SIZE 1024 

int  fd, timed;
time_t prev;
pid_t childPID;

// Unios for converting compressed integers into C integers.
union ISTR
{
    char bytes[4];
    int val;
};

// Funcion for checking the return status of a systemcall
void checkError(int err, char *str)
{
    if(err == -1)
    {
        perror(str);
        exit(EXIT_FAILURE);
    }
}

// Exit Handler, ensures that every child terminates in absence of the parent.
void exitP(void)
{
    kill(childPID, SIGTERM);
}

// Parent Signal Handler
void signalHandlerP(int sig)
{
    char out[BUF_SIZE];
    time_t curr;
    char in[2];

    // If child has exited, alert user and exit parent
    if(sig == SIGCHLD)
    {
        sprintf(out,"\nWarning! Pressure control monitoring has been terminated, exiting the system\n");
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        exit(EXIT_SUCCESS);
    }

    // Handle SIGINT by asking user for conformation before exiting
    if(sig == SIGINT)
    {
        sprintf(out, "\nExit: Are you sure (Y/n): ");
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");

        checkError(read(STDIN_FILENO, in, 2), "read"); 
        
        if(in[0] == 'y' || in[0] == 'Y')
            exit(EXIT_SUCCESS);

        sprintf(out, "\n");
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        
        return;
    }

    // If sig user 1 is recieved, alert user that pressure is low and display time since last alert.
    if(sig == SIGUSR1)
    {
        time_t curr = time(NULL);
        if(timed == 0)
        {
            prev = curr;
            timed = 1;
        }
        sprintf(out, "\nWarning! Line pressure is dangerously low, it has been %ld seconds since the previous warning\n", curr - prev);
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        prev = curr;
        return;
    }

    // If sig user 2 is recieved, alert user that pressure is high and display time since last alert.
    if(sig == SIGUSR2)
    {
        time_t curr = time(NULL);
        if(timed == 0)
        {
            prev = curr;
            timed = 1;
        }
        sprintf(out, "\nWarning! Line pressure is dangerously high, it has been %ld seconds since the previous warning\n", curr - prev); 
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        prev = curr;
        return; 
    }
}

// Child Signal Handler
void signalHandlerC(int sig)
{
    int ret;
    char out[BUF_SIZE];
    pid_t ppid = getppid();
    union ISTR in;

    // Ignore sig int
    if(sig == SIGINT)
        return;

    // On SIGTERM, exit child
    if(sig == SIGTERM)
        exit(EXIT_SUCCESS);
     
    // On sig alarm, read value from file and determine whether pressure is low, high, or normal, send appropriate signal to parent.
    if(sig == SIGALRM)
    {
        ret = read(fd, in.bytes, 2);
        checkError(ret, "read");
        if(ret == 0)
        {
            close(fd);
            exit(EXIT_SUCCESS);
        }
        if(in.val < 550)
        {
            kill(ppid, SIGUSR1);           
        }
        if(in.val > 1450)
        {
            kill(ppid, SIGUSR2);           
        }
    }
}

// Function for reading a line.
int readLine(int fd, char *line)
{
    char buf [1024];
    char cha = ' ';
    int state;
    int i = 0;
    while(cha != '\n' && cha != '\r' && cha != '\0')
    {
        state = read(fd, &cha, 1);
        if(state == -1 || state == 0)
            return 0;
        if(cha == '\0')
            break;
        if(cha == '\r')
            break;
        if(cha == '\n')
            break;
        if(cha == EOF)
            break;
        buf[i] = cha;
        ++i;
    }
    buf[i] = '\0';
    strcpy(line, buf);
    if(i == 0)
        return 0;
    return 1;
}

int main()
{
    char out[BUF_SIZE];
    char in[BUF_SIZE];

    // Signal handler and time struct declerations.
    struct sigaction saP;
    struct sigaction saC;
    struct itimerval tv;

    // Signal handler initializations
    saP.sa_handler = signalHandlerP;
    sigemptyset(&saP.sa_mask);
    saP.sa_flags = 0;

    saC.sa_handler = signalHandlerC;
    sigemptyset(&saC.sa_mask);
    saC.sa_flags = 0;

    // Timeval struct initialization
    tv.it_value.tv_sec = 2;
    tv.it_value.tv_usec = 0;
    tv.it_interval.tv_sec = 2;
    tv.it_interval.tv_usec = 0;

    // Spawn cihld
    switch(childPID = fork())
    {
        // If fork fails, exit
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        //Child Process
        case 0:
            // Assign signal handlers and set up masks.
            sigaddset(&saC.sa_mask, SIGINT);    
            checkError(sigaction(SIGINT, &saC, NULL), "sigaction");
            checkError(sigaction(SIGTERM, &saC, NULL), "sigaction");
            checkError(sigaction(SIGALRM, &saC, NULL), "sigaction");

            // Open file
            fd = open("flowdata.dat", O_RDONLY);
            checkError(fd, "open"); 

            // Start timer
            checkError(setitimer(ITIMER_REAL, &tv, NULL), "setitimer");

            while(1)
            {
                pause();
            }
                
    }

    // Setup exit handler
    atexit(exitP);

    
    // Assign signal handler and mask.
    sigaddset(&saP.sa_mask, SIGINT);   
    sigaddset(&saP.sa_mask, SIGUSR1); 
    sigaddset(&saP.sa_mask, SIGUSR2); 
    checkError(sigaction(SIGCHLD, &saP, NULL), "sigaction");
    checkError(sigaction(SIGINT, &saP, NULL), "sigaction");
    checkError(sigaction(SIGUSR1, &saP, NULL), "sigactoin");
    checkError(sigaction(SIGUSR2, &saP, NULL), "sigaction");

    // While program runs, prompt user for input, and output it to STDOUT
    while(1)
    {    
        sprintf(out, "Please enter some input: ");        
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        sprintf(out, "");

        readLine(STDIN_FILENO, in);
        
        checkError(write(STDOUT_FILENO, in, strlen(in)), "write");
        sprintf(in, "");

        sprintf(out, "\n");
        checkError(write(STDOUT_FILENO, out, strlen(out)), "write");
        sprintf(out, "");
    }
}
