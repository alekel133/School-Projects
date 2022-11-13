/*
    Author: Alexander Kellough
    NetID: atk133
    Compiler: gcc
    Description: Basic Tic Tac Toe Program
 */
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

// Binary sem globals
int bsUseSemUndo;
int bsRetryOnEintr;

// Board struct
typedef struct board
{
    int count;
    int board[3][3];
}board;

// Semun Struct
typedef union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
}semun;

// Binary Sem functions
int initSemAvailable(int semId, int semNum)
{
    semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum)
{
    semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum)
{
    struct sembuf sops;
    sops.sem_num = semNum; 
    sops.sem_op = -1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    while(semop(semId, &sops, 1) == -1)
    {
        if(errno != EINTR || !bsRetryOnEintr)
            return -1;
    }

    return 0;
}

// Check for tie
int checkTie(board* board)
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(board->board[i][j] == 0)
                return 0;
        }
    } 
    return 1;
}

// Check for win
int checkWin(board* board)
{
    int sum;
    for(int i = 0; i < 3; ++i)
    {
        sum = 0;
        for(int j = 0; j < 3; ++j)     
        {
           sum += board->board[i][j]; 
        }
        if(sum == 3)
        {
            return 1;
        }
        if(sum == -3)
        {
            return -1;
        }
    } 

    for(int j = 0; j < 3; ++j)
    {
        sum = 0;
        for(int i = 0; i < 3; ++i)     
        {
           sum += board->board[i][j]; 
        }
        if(sum == 3)
        {
            return 1;
        }
        if(sum == -3)
        {
            return -1;
        }
    }

    sum = board->board[0][0] + board->board[1][1] + board->board[2][2];
    if(sum == 3)
    {
        return 1;
    }
    if(sum == -3)
    {
        return -1;
    }

    sum = board->board[0][2] + board->board[1][1] + board->board[2][0];
    if(sum == 3)
    {
        return 1;
    }
    if(sum == -3)
    {
        return -1;
    }
    return 0;
}

// Release sem
int releaseSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO:0; 

    return semop(semId, &sops, 1);
}

// Check system call
void checkCall(int ret, char* str)
{
    if(ret != -1)
        return;

    perror(str);
    exit(EXIT_FAILURE);
}

// Display board
void displayBoard(board* b_ptr)
{
    for(int i = 0; i < 3; ++i)
    {
        printf("[ ");
        for(int j = 0; j < 3; ++j)
        {
            if(b_ptr->board[i][j] == 0)
                printf("  ");
            if(b_ptr->board[i][j] == -1)
                printf("O ");
            if(b_ptr->board[i][j] == 1)
                printf("X ");
        }    
        printf("]\n");
    }
    printf("\n");
}

// Move for x
void makeMoveX(board* board)
{
    int x, y;
    while(1)
    {
        x = rand() % 3;
        y = rand() % 3;
        if(board->board[x][y])
        {
           continue; 
        }
        board->board[x][y] = 1;
        break;
    }
}

// Move for O 
void makeMoveO(board* board)
{
    int x, y;
    while(1)
    {
        x = rand() % 3;
        y = rand() % 3;
        if(board->board[x][y])
        {
           continue; 
        }
        board->board[x][y] = -1;
        break;
    }
}

int main(int argc, char *argv[])
{
    int player, sem_key, shm_key, fd, shm_id, sem_id, shm_val, sem_val, win;
    void * shmat_ret;
    board* shm_point;
    board copy;
    struct stat buffer;

    srand(time(NULL));

    // Check args
    if(argc < 2)
    {
        perror("Syntax maij.c <player num>");
        exit(EXIT_FAILURE);
    }

    // Read args
    if(argc >= 2)
    {
        player = atoi(argv[1]);    
    }
    
    // If player is x
    if(player == -1)
    {
        // If xoSync doesn't exist, create it
        if(stat("xoSync", &buffer) == -1)
        {
            checkCall( mkfifo("./xoSync", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP), "mkfifo");
        }

        // generate random values for keys
        shm_val = rand();
        sem_val = rand();

        // generate keys
        shm_key = ftok("./xoSync", shm_val);
        sem_key = ftok("./xoSync", sem_val);
        checkCall(shm_key, "ftok");
        checkCall(sem_key, "ftok");

        // Make shared memory
        shm_id = shmget(shm_key, sizeof(board), 0660 |IPC_CREAT); 
        checkCall(shm_id, "shmeget");

        // Make semaphores
        sem_id = semget(sem_key, 2, IPC_CREAT);
        checkCall(sem_id, "semget");

        // Initialize semaphores
        initSemAvailable(sem_id, 0);
        initSemInUse(sem_id, 1);

        //Attach shared memory
        shmat_ret = shmat(shm_id, NULL, 0);
        checkCall((long)shm_point, "shmat");

        // Make shared memory compatible with board struct 
        shm_point = (board*) shmat_ret; 
        *shm_point = copy;
        
        // Initialize shared memory
        shm_point->count = 0;
        for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < 3; ++j)
            {
                shm_point->board[i][j] = 0;
            }
        }
        
        // Use FIFO to sync programs and send key data
        fd = open("xoSync", O_WRONLY);
        checkCall(fd, "open");
        checkCall(write(fd, &shm_val, sizeof(shm_val)), "write");
        checkCall(write(fd, &sem_val, sizeof(sem_val)), "write");
        checkCall(close(fd), "close");

        // Enter gameplay loop
        while(shm_point->count > -1)
        {
            reserveSem(sem_id, 0);
            displayBoard(shm_point); 
            makeMoveX(shm_point);;
            ++shm_point->count;
            displayBoard(shm_point);

            if(checkWin(shm_point))
            {
                printf("X wins!\n");
                shm_point->count = -1;
                win = 1;
            }

            if(checkWin(shm_point) == -1)
            {
                printf("O wins!\n");
                shm_point->count = -1;
                win = 1;
            }

            if(checkTie(shm_point) && !win)
            {
                printf("It's a tie\n");
                shm_point->count = -1;
            }
            releaseSem(sem_id, 1);
        }

        // Use FIFO to sync programs
        fd = open("xoSync", O_WRONLY);
        close(fd);
        
        // Delete/Detach shared memory and semaphores
        checkCall(semctl(sem_id, 0, IPC_RMID), "semctl");
        checkCall(shmdt(shmat_ret), "shmdt");
        checkCall(shmctl(shm_id, IPC_RMID, NULL), "shmctl");
    }

    if(player == -2)
    {
        // If FIFO doesn't exist, create it.
        if(stat("xoSync", &buffer) == -1)
        {
            checkCall( mkfifo("./xoSync", O_RDWR | O_CREAT | O_TRUNC), "mkfifo");
        } 

        // Sync programs, get key data
        fd = open("xoSync", O_RDONLY);
        checkCall(fd, "open");
        checkCall(read(fd, &shm_val, sizeof(shm_val)), "read");
        checkCall(read(fd, &sem_val, sizeof(sem_val)), "read");
        checkCall(close(fd), "close");
        shm_key = ftok("./xoSync", shm_val);
        checkCall(shm_key, "ftok");
        sem_key = ftok("./xoSync", sem_val);
        checkCall(sem_key, "ftok");

        // get shared memory
        shm_id = shmget(shm_key, sizeof(board), 0660);
        checkCall(shm_id, "shmget");

        // get semaphores
        sem_id = semget(sem_key, 2, IPC_CREAT);
        checkCall(sem_id, "semget");

        // Attach shared memory
        shmat_ret = shmat(shm_id, NULL, 0);
        checkCall((long)shmat_ret, "shmat");
        shm_point = (board*)shmat_ret;
        // Enter Gameplay Loop
        while(shm_point->count > -1)
        {
            reserveSem(sem_id, 1);
            displayBoard(shm_point);
            makeMoveO(shm_point);
            ++shm_point->count;
            displayBoard(shm_point);

            if(checkWin(shm_point) == 1)
            {
                printf("X wins\n");
                shm_point->count = -1;
                win = 1;
            }

            if(checkWin(shm_point) == -1)
            {
                printf("O wins!\n");
                shm_point->count = -1;
                win = 1;
            }

            if(checkTie(shm_point) && !win)
            {
                printf("It's a tie\n");
                shm_point->count = -1;
            }
            releaseSem(sem_id, 0);
        } 

        // Sync programs and detach shared memory 
        fd = open("xoSync", O_RDONLY);
        checkCall(fd, "open");
        checkCall(close(fd), "close");
        checkCall(shmdt(shmat_ret), "shmdt");
    }
} 
