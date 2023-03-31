/*
   Author: Alexander Kellough
   NetID: atk133
   
Description:
   Small program for emulating wc -l command using pthread.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// Global Line Counter
int numLines = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Update numLines
void updateCount(int newLines)
{
    // Lock mutex -> append line_count-> Unlock Mutex
    pthread_mutex_lock(&lock);
    numLines += newLines;
    pthread_mutex_unlock(&lock);
}

// Function to count lines
void *countLines(void *filename)
{
    int count;
    char buf;
    FILE* file;
    
    // Attempt to open file, and exit if error
    file = fopen((char *) filename, "r");
    if(file == NULL)
    {
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        // Loop until end of file counting newlines
        if(feof(file))
        {
            break;
        }

        buf = fgetc(file);

        if(buf == '\n')
            ++count; 
    }

    // Race Condition
    updateCount(count);
    return NULL;
}

// Main Function
int main(int argc, char *argv[])
{
    int threadCount = argc - 1;
    pthread_t *threads;
    int *rec;

    if(argc < 2)
    {
        perror("Error: Syntax (linenumber <file1> ... <file n>)");
        exit(EXIT_FAILURE);
    }

    // Sets up thread object & return variable vectors size = len(argv-1) = num_files
    threads = (pthread_t *) malloc((threadCount) * sizeof(pthread_t));
    rec = (int *) malloc((threadCount) * sizeof(int));
    
    for(int i = 0; i < threadCount; ++i)
    {
        // Create threads and report errors
        if( (rec[i] = pthread_create(&threads[i], NULL, countLines, argv[i+1])) ) 
        {
            perror("Error in pthread_create ");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < threadCount; ++i)
    {
        // Roert errors in joining of threads
        if(pthread_join(threads[i], NULL))
        {
          perror("Erro in pthread_join");
          exit(EXIT_FAILURE);
        }
    }

    // print number of lines and exit
    fprintf(stdout, "%d\n", numLines);
    exit(EXIT_SUCCESS);
}
