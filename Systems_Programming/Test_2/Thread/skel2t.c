
/* 
 *  Author: Alexander Kellough
 *  NetID: atk133
 *  Compiler: GCC
 *
 * Write a threaded program that does the following
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into 
      a file named data.dat
   3. Determines how many integers are > 100
   4. Determines how many integers are < 100
   5. Outputs the total count for each group.

   The program should perform this task in the following way:
   Create a producer thread that:
   1. Prompts the user for the number of integers to enter
   2. Prompts the user for each integer and writes them into a file
      named data.dat
   Create a consumer thread that:
   1. Determines how many integers are > 100
   2. Outputs that value
   3. Sets that value to its exit value
   Create a consumer thread that:
   1. Determines how many integers are < 100
   2. Outputs that value
   3. Sets that value to its exit value
   Create a main thread that: 
   1. Creates threads 1 - 3
   2. Waits on the values of threads 2 and 3. 
   3. Outputs the values from threads 2 and 3.
 */

/* Include files */
#include <stdio.h>
#include <stdlib.h>
/* Problem 1. Add the necessary include files for this program */
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>


/* 
   Global variables:
   We will need a mutex, a condition variable, and a predicate variable. 
   Recall that the predicate variable is the variable we use to determine
   whether data was available prior to our first call to pthread_cond_wait

 */

/* Problem 2. Declare the global variables for the predicate variable, 
   the mutex, and the condition variable. Do no forget to initialize 
   the mutex and the condition variable 
 */
typedef struct retData
{
    int ret;
} retData;

int predicate = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;



/* This is a convenience function for dealing with errors
   and threads
 */

void hndlError(int error, const char *str)
{
  if (error == 0) return;
  errno = error;
  perror(str);
  exit(EXIT_FAILURE);
}


/* Define the three thread start functions. 
   You can name them whatever you wish 
 */

/* Problem 3. Define and write the start function for thread 1 */
void * startThread1()
{
    int tot, val, fd, ret;

    pthread_detach(pthread_self());
    printf("How many integers would you like to enter: ");
    scanf("%d", &tot);
    printf("\n");

    ret = pthread_mutex_lock(&mtx);
    if(ret == -1)
    {
        perror("mutext_lock");
        pthread_exit(&ret);
    }

    fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if(fd == -1)
    {
        perror("open");
        ret = -1;
        pthread_exit(&ret);
    }

    for(int i = 0; i < tot; ++i) 
    {
        printf("Intiger %d: ", i+1);
        scanf("%d", &val);
        printf("\n");
        ret = write(fd, &val, 4);
        if(ret == -1)
        {
            perror("write");
            pthread_exit(&ret);
        }
    }

    close(fd);
    predicate = 1;
    ret = pthread_mutex_unlock(&mtx);
    if(ret == -1)
    {
        perror("mutex_unlock");
        pthread_exit(&ret);
    } 

    ret = pthread_cond_broadcast(&cond);
    if(ret == -1)
    {
        perror("cond_brodacast");
        pthread_exit(&ret);
    }
    pthread_exit(NULL);
}

/* Problem 4. Define and write the start function for thread 2 */
void *startThread2(void *arg)
{
    retData *argval = (retData *) arg;
    int s, fd, val;
    int cnt = 0;
   
    s = pthread_mutex_lock(&mtx); 
    if(s == -1)
    {
        perror("mutex_lock");
        pthread_exit(&s);
    }

    while(predicate == 0)
    {
        s = pthread_cond_wait(&cond, &mtx);
        if(s == -1)
        {
            perror("cond_wait");
            pthread_exit(&s);
        }
    }

    fd = open("data.dat", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        pthread_exit(&fd);
    }
    
    s = pthread_mutex_unlock(&mtx);
    if(s == -1)
    {
        perror("cond_wait");
        pthread_exit(&s);
    }

    while(read(fd, &val, 4)) 
    {
        if(val > 100)
            ++cnt;
    }

    close(fd);
    argval->ret = cnt;
    pthread_exit(NULL);
}

/* Problem 5. Define and write the start function for thread 3 */
void *startThread3(void *arg)
{
    retData *argval = (retData *) arg;
    int s, fd, val;
    int cnt = 0;
   
    s = pthread_mutex_lock(&mtx); 
    if(s == -1)
    {
        perror("mutex_lock");
        pthread_exit(&s);
    }

    while(predicate == 0)
    {
        s = pthread_cond_wait(&cond, &mtx);
        if(s == -1)
        {
            perror("cond_wait");
            pthread_exit(&s);
        }
    }

    fd = open("data.dat", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        pthread_exit(&fd);
    }
    
    s = pthread_mutex_unlock(&mtx);
    if(s == -1)
    {
        perror("cond_wait");
        pthread_exit(&s);
    }

    while(read(fd, &val, 4)) 
    {
        if(val < 100)
            ++cnt;
    }

    close(fd);
    argval->ret = cnt;
    pthread_exit(NULL);
}

/* Problem 6: Define and write the function for the main thread */
int main(int argc, char *argv[])
{
    int s, large, small;
    retData sn1, sn2;
    pthread_t t1, t2, t3;

    s = pthread_create(&t1, NULL, startThread1, NULL);
    if(s != 0)
    {
        errno = s;
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t2, NULL, startThread2, &sn1);
    if(s != 0)
    {
        errno = s;
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t3, NULL, startThread3, &sn2);
    if(s != 0)
    {
        errno = s;
        perror("pthread_create");
        exit(EXIT_FAILURE);
    } 

    s = pthread_join(t2, NULL); 
    if(s != 0)
    {
        errno = s;
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t3, NULL);
    if(s == -1)
    {
        errno = s;
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    large = (int)sn1.ret;
    small = (int)sn2.ret;

    printf("Larger: %d\n", large);
    printf("Smaller: %d\n", small); 

    exit(EXIT_SUCCESS); 
}
