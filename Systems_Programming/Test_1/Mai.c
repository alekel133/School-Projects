/*All of the includes fot the project go here ...
 * those include must support signals, inerval timers,
 * read, open, and close. You will also need support for
 * error numbers
 */
#include <stdio.h>
#include <stdlib.h>
/*Problem 1 - complete the list of include files.*/

#define BUF_SIZE 1024

static int timed;

/*the myPrint functions takes a constant c-string
 * and outputs it to STDOUT using write
 * feel free to use string functions like strcmp, strcat, etc.
 * to make this one work
 * myPrint returns 0 on failure and 1 on success.
 */ 
int myPrint(const char *str)
{
/* Problem 2 - implement myPrint using write */
return timed; // This is just so the autocorrect will leave me alone.
}

/*the myPrintInt function takes a constant int
 * and outputs it to STDOUT using write
 * feel free to use of a function like sprintf to convert
 * an integer to a string
 * myPrintInt returns 0 failure and a 1 on success
 * Note you will need to convert int to string
 * to make it writeable.
 */ 
int myPrintInt(const int val)
{
/* Problem 3 - Implement myPrintInt, you mus use write or another function implented in this program*/
return timed; // Just so auto-correct will leave me on.
}

/*Implement the signal handler below...
 * use provided prototype
 * It must handle signals, SIGINT and SIGALARM
 * if the signal handler recieves SIGINT
 * prompt the user if the want to exit or not
 * on yes, exit succesfully, on no, just return from the handler
 * on SIGALRM, set the timeout flag to 1
 */
void signalHandler(int sig)
{
/*Problem-4 implement the signalHandler...
 * when writing the prompt for SIGINT and reading the reply
 * use system calls... please also remember to read more than 1
 * character when reading response since that response has at least
 * two characters in it. Yes the responses of "y" and "n" involve multiple characters.
 * Remember, that functions like printf and scanf aren't considered interupt handler
 * safe, use system call or functons written in this program in their place.
 */
}

/*This function reads a line of text from a file
 * we are simulating another function behavior here...
 * the basic idea is to read a file character by character until
 * we find the \n or \r
 * Go ahead and tack the null characters on here for simplicity
 * Return a 0 if no characters were read, return a 1 otherwise.
 */
int readLine(int fd, char * line)
{
/*Problem-5 implement readLine as described above... again
 * remember to use read to do this... This is probably bes accomplished by
 * reading in 1 characterat at time and then adding them to the array
 * you passed in as line.
 */
 return timed;
}

/*This function reads a question answer ppairing
 * from the provided pair of file descriptors
 * It returns 0 if the files are empty
 * and 1 if it successfully reads the pairing
 * notice it calls the custom function readLine since we
 * need to read only one line at a time.
 */
int readQA(int questFd, int ansFd, char *quest, char *ans)
{
if(readLine(questFd,quest) == 0) return 0;
if(readLine(ansFd, ans) == 0) return 0;
return 1;
}

int main(int argc, char *argv[])
{
    int numRead = 0;
    int numWrite = 0;
    int question = 1;
    int correct = 0;
    char buf[BUF_SIZE];
    char quest[BUF_SIZE];
    char ans[BUF_SIZE];
    int questFd, ansFd;

    /* declare structures for signals and timers,
     * recall that you can only set one REAL timer at at time,
     * but that will need to set two different delays... the active delay of 50 seconds
     * and the disable delay of 0 seconds. Treat each delay as a different instance of the timer
     * structure. For simplicity, name the active delay structure tvOn and the disabled delay
     * structure tvOff.
     */

    /*Problem 6a-declare the structures for the signals and timers.*/

    /*Initialize structures for signals and timers
     * make certain to actually set a mask this time
     * add both SIGINT and SIGALRM to the mask.
     */

    /*Problem 6b - initialize the structures for the signal handler.*/







}
