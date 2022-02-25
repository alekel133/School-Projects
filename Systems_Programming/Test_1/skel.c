/*
 *  Author: Alexander Kellough
 *  NetID: 133
 *  Compiler: GCC
 *  Description:
 *      Modified skel.c code corresponding to the programming portion of Sytems Programming, Prof. Aubrey Knight
 */

/*All of the includes for the project go here...
 * those includes must support signals, interval timers,
 * read, open, and close. you will also need support for 
 * error numbers
 */

#include <stdio.h>
#include <stdlib.h>

/* Problem 1 */
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>

#define BUF_SIZE 1024

static int timed;

/* the myPrint fucntion takes a constant c-string
 * and outputs it to STDOUT using write
 * myPrint returns 0 on failure and 1 on success
 */
int myPrint(const char *str)
{
/* Problem 2 */
    if(write(1, str, strlen(str)) == -1)
        return 0;

    return 1;
}

/* the myPrintInt function takes a constant int
 * and outputs it to STDOUT using write
 * myPRintInt retursn 0 on failure and 1 on success
 */
int myPrintInt(const int val)
{
/* Problem 3 */
    char *str = (char *) malloc(4);
    sprintf(str, "%d", val);
    if(myPrint(str) == 0)
        return 0;
    
    return 1;
}

/* implement the signal handler below ...
 * use the provided prototype
 * It must handle two signals, SIGINT and SIGALRM
 */
void signalHandler(int sig)
{
/* Problem 4 */
    char reply[2];
    if(sig == SIGINT)
    {
        myPrint("\nDo you wnat to exit? (y/n): ");
        if(read(0, reply, 2) == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(reply[0] == 'Y' || reply[0] == 'y')
        {
            exit(EXIT_SUCCESS);
        }
        else
            return;
    }
    if(sig == SIGALRM)
    {
        timed = 1;
    }
    return;
}

/* This function reads a line of text from a file
 * we are simulating another function behavior here ...
 * Reurn a 0 if no characters were read, return a 1 otherwise
 */
int readLine(int fd, char *line)
{
/* Problem 5 */
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

/* This function reads a question answer pairing
 * from the provided pair of file descriptors
 * It returns 0 if the files are empty
 * and 1 if it usccessfully reads the pairing
 */
int readQA(int questFd, int ansFd, char *quest, char *ans)
{
    if(readLine(questFd, quest) == 0) return 0;
    if(readLine(ansFd, ans) == 0) return 0;
    return 1;
}

int main(int argc, char* argv[])
{
    int questFd;
    int ansFd, numRead;
    int question = 1;
    int correct = 0;
    char buf[BUF_SIZE]; 
    char quest[BUF_SIZE];
    char ans[BUF_SIZE];

    /* Problem 6a */
    struct sigaction sa;
    struct itimerval tvOn, tvOff;
    
    /* Problem 6b */
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGALRM);
    sa.sa_handler = signalHandler;
    sa.sa_flags = 0;
    
    /* Problem 6c */
    tvOn.it_value.tv_sec = 30;
    tvOn.it_value.tv_usec = 0;
    tvOn.it_interval.tv_sec = 0;
    tvOn.it_interval.tv_usec = 0;

    /* Problem 6d */
    tvOff.it_value.tv_sec = 0;
    tvOff.it_value.tv_usec = 0;
    tvOff.it_interval.tv_sec = 0;
    tvOff.it_interval.tv_usec = 0;


    /* Problem 7*/
    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGALRM, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    /* Problem 8 */
    questFd = open("quest.txt", O_RDONLY);
    if(questFd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
    ansFd = open("ans.txt", O_RDONLY);
    if(ansFd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }
   
    /* this loop handles the Q/A stuff
     * I have included some of it to make your life simpler
     * I have also left some comments to assisst you as well
     *
     * rread the first question, answer pairing prior to entering the loop
     */
    readQA(questFd, ansFd, quest, ans);

    while(1)
    {
        /* Output the current question */
        myPrint("#");
        myPrintInt(question);
        myPrint(" "); 
        myPrint(quest);
        myPrint("? ");

        /* we will set the timedout flag to 0, since it hasn't yet */
        timed = 0;

        /* now set the interval timer prior to reading in the user's response */

        /* Problem 9 */
        if(setitimer(ITIMER_REAL, &tvOn, NULL) == -1)
        {
            perror("setitimer");
            exit(EXIT_FAILURE);
        }

        /* read in the user's response, this is systems programming so don't use 
         * scanf */

        /* Probelm 10 */
        memset(buf, 0,BUF_SIZE);
        numRead = read(0, buf, sizeof(ans));

        /* test to see if the user responded and if an error has occurred
         * an eroor cna actually occur as part of the functionality of this program
         * both SIGINT and SIGALRM will cause the read acall to return a -1
         * this can be recognized by testing for a specific error number ...
         * The error is known as interrupted system call
         * Should this be the case, do the following ...
         * If the read call was interrupted and the timedout flag isn't set
         * just continue
         * if the timedout flag was set, inform the user that time is up
         * then go to the next question
         */
        if(numRead == 0) break;
        if(numRead == -1)
        {
            if(errno == EINTR)
            {
                if(timed)
                {
                    myPrint("\nTime's up, next question\n");
                    if(readQA(questFd, ansFd, quest, ans) == 0) break; 
                    question++;
                }
                continue;
            }
            perror("Read");
            exit(EXIT_FAILURE);
        }
        /* disable the timer here */

        /* Problem 12 */
        if(setitimer(ITIMER_REAL, &tvOff, NULL) == -1) 
        {
            perror("setitimer");
            exit(EXIT_FAILURE);
        } 

        /* we will convert the buf being read into a c-string by tacking on a 0 */
        buf[numRead -1] = 0;

        /* CHeck the answer */
        if(strcmp(buf, ans) == 0)
        {
            correct++;
            myPrint("correct\n");
        }
        else
        {
            myPrint(ans);
            myPrint("\nwrong\n");
        }

        /* read the next question ... break if there are no more questions */
        if(readQA(questFd, ansFd, quest, ans) == 0) break;

        question++; 
    }

    myPrint("final score is ");
    myPrintInt(correct);
    myPrint(" out of ");
    myPrintInt(question);
    
    /* Problem 12 */
    close(questFd);
    close(ansFd);

    exit(EXIT_SUCCESS);

}
