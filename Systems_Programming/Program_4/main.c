#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include "binary_sem.h"

typedef struct board
{
    int count;
    int board[3][3];
}board;

void checkCall(int ret, char* str)
{
    if(ret != -1)
        return;

    perror(str);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int player, sem_key, shm_key, fd, shm_id, sem_id;
    void * shmat_ret;
    board* shm_point;
    board copy;
    struct stat buffer;

    srand(time(NULL));

    if(argc < 2)
    {
        perror("Syntax maij.c <player num>");
        exit(EXIT_FAILURE);
    }

    if(argc >= 2)
    {
        player = atoi(argv[1]);    
    }
    
    if(player == -1)
    {
        if(stat("xoSync", &buffer) == -1)
        {
            checkCall( mkfifo("./xoSync", O_RDWR | O_CREAT | O_TRUNC), "mkfifo");
        }

        shm_key = rand();
        sem_key = rand();
        shm_key = ftok("./xoSync", shm_key);
        sem_key = ftok("./xoSync", sem_key);
        checkCall(shm_key, "ftok");
        checkCall(sem_key, "ftok");
        shm_id = shmget(shm_key, sizeof(board), 0660 |IPC_CREAT); 
        checkCall(shm_id, "shmeget");
        sem_id = semget(sem_key, 2, IPC_CREAT);
        checkCall(sem_id, "semget");
        initSemiAvailable(sem_id, 1);
        initSemInUse(sem_id, 2);
        shmat_ret = shmat(shm_id, NULL, 0);
        checkCall((long)shm_point, "shmat");
        shm_point = (board*) shmat_ret; 
        shm_point = &copy;
        shm_point->count = 0;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                shm_point->board[i][j] = 0;
            }
        }
        
        checkCall(semctl(sem_id, 0, IPC_RMID), "semctl");
        checkCall(shmdt(shmat_ret), "shmdt");
        checkCall(shmctl(shm_id, IPC_RMID, NULL), "shmctl");
    }
} 
