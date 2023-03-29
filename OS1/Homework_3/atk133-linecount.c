#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int numLines = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Update numLines
void updateCount(int newLines)
{
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
    file = fopen((char *) filename, "r");
    if(file == NULL)
    {
        perror("Error Opening File");
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        if(feof(file))
        {
            break;
        }

        buf = fgetc(file);

        if(buf == '\n')
            ++count; 
    }
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

    threads = (pthread_t *) malloc((threadCount) * sizeof(pthread_t));
    rec = (int *) malloc((threadCount) * sizeof(int));
    
    for(int i = 0; i < threadCount; ++i)
    {
        if( (rec[i] = pthread_create(&threads[i], NULL, countLines, argv[i+1])) ) 
        {

            perror("Error in thread creation: ");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < threadCount; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    fprintf(stdout, "%d\n", numLines);
    exit(EXIT_SUCCESS);
}
