/* Author: Alexander Kellough
 * NetID: atk133
 * Compiler: gcc (GCC) 12.2.1
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/stat.h>

// Check system calls for errors (Credit: Aubrey Knight).
void checkError(int err, char *prompt){
    if(err == -1){
        perror(prompt);
        exit(EXIT_FAILURE);
    }
}

// Prints data from the stat structure
void printStat(struct stat *buf){
    struct tm *atime;
    struct tm *mtime;
    struct tm *ctime;
    char dirstat[4]; 

    atime = localtime(&buf->st_atime);
    mtime = localtime(&buf->st_mtime);
    ctime = localtime(&buf->st_ctime);

    if(S_ISDIR(buf->st_mode) != 0){
        dirstat[0] = 'y';
        dirstat[1] = 'e';
        dirstat[2] = 's';
        dirstat[3] = (char) 0;
    }
    else
    {
        dirstat[0] = 'n';
        dirstat[1] = 'o';
        dirstat[3] = (char) 0;
    }

    printf("\tIs Directory: %s\n", dirstat); 
    printf("\tUser ID: %d\n", buf->st_uid); 
    printf("\tGroup ID: %d\n", buf->st_gid); 
    printf("\tNumber of Links: %ld\n", buf->st_nlink); 
    printf("\tTime of Last Access: %s", asctime(atime)); 
    printf("\tTime of Last Modification: %s", asctime(mtime)); 
    printf("\tTime of Last Status:  %s", asctime(ctime)); 
    printf("\tSize (bytes):  %ld\n", buf->st_size); 
    printf("\tInode Number:  %ld\n", buf->st_ino); 
    puts("");
}

int main(int argc, char *argv[]){
    int fd;
    unsigned long long userTime, kernelTime;
    struct stat statbuf;
    struct rusage usagebuf;

    if(argc < 2){ 
        perror("Error: Not enough filenames");
        exit(EXIT_FAILURE);
    }
    
    if(argc > 51){
        perror("Error: Too many filenames.");
        exit(EXIT_FAILURE);
    }

    for(int i = 1; i < argc; ++i){ // Iterate through files in argv
        fd = open(argv[i], S_IRUSR);
        checkError(fd, "Error: open()");
    
        checkError(fstat(fd, &statbuf), "Error: fstat()");    
        printf("\"%s\":\n", argv[i]);
        printStat(&statbuf);

        close(fd);
    }

    checkError(getrusage(RUSAGE_SELF, &usagebuf), "Error: getrusage()"); // Get usage and noramlize times to microseconds
    userTime = usagebuf.ru_utime.tv_sec * 1000000L + usagebuf.ru_utime.tv_usec; 
    kernelTime = usagebuf.ru_stime.tv_sec * 1000000L + usagebuf.ru_stime.tv_usec;

    puts("");

    printf("User Time: %lld (us)\n", userTime);
    printf("Kernel Time: %lld (us)\n", kernelTime);

    exit(EXIT_SUCCESS);
}
