#include <stdio.h>
#include <stdlib.h> 
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

int bsUseSemUndo;
int bsRetryOnEintr;
pid_t childPID;

typedef struct lc_pair
{
    int len;
    char type;
}lc_pair;

typedef struct params
{
    int num_blocks; 
    int block_length;
    lc_pair *chars;
}params;

typedef union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
}semun;

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

int releaseSem(int semId, int semNum)
{
    struct sembuf sops;
    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    return semop(semId, &sops, 1);
}

void exitP()
{
    kill(childPID, SIGTERM);
}

void checkCall(int err, char *str)
{
    if(err == -1)
    {
        perror(str);
        exit(EXIT_FAILURE);
    }
}

int getRand(int lower_limit, int upper_limit)
{
    int val;
    val = rand();
    val = val % (upper_limit + 1);

    if(val < lower_limit)
        val = val + lower_limit;

    return val;
}

int child(int shm_id, int sem_id)
{
    void *mat_ret;
    params cpy;
    params* shm_point;

    srand(time(NULL));

    checkCall(reserveSem(sem_id, 1), "reserveSem");
    
    mat_ret = shmat(shm_id, NULL, 0);
    checkCall((long)mat_ret, "shmat");

    shm_point = (params*) mat_ret;
    *shm_point = cpy;

    shm_point->num_blocks = getRand(10, 20);
    printf("Child %d\n", shm_point->num_blocks);

    lc_pair tmp[shm_point->num_blocks];

    for(int i = 0; i < shm_point->num_blocks; ++i)
    {
        tmp[i].len = getRand(2,10); 
        tmp[i].type = (getRand(0,25) + 97);
    }

    shm_point->chars = tmp;

    checkCall(releaseSem(sem_id, 0), "releseSem");
    checkCall(reserveSem(sem_id, 1), "reserveSem");

    checkCall(shmdt(mat_ret), "shmdt");

    checkCall(releaseSem(sem_id, 0), "releaseSem");
    exit(EXIT_SUCCESS); 
}

int parent(int shm_id, int sem_id)
{
    void *mat_ret;
    params cpy;
    params* shm_point;
   
    srand(time(NULL));
        
    mat_ret = shmat(shm_id, NULL, 0);
    checkCall((long)mat_ret, "shmat");
    
    shm_point = (params*) mat_ret;

    checkCall(reserveSem(sem_id, 0), "reserveSem");

    for(int i = 0; i < shm_point->num_blocks; ++i)
    {
        for(int j = 0; j < shm_point->chars[i].len; ++j)
        {
            printf("%c", shm_point->chars[i].type);
        }
    }
    
    checkCall(releaseSem(sem_id, 1), "releaseSem");
    checkCall(reserveSem(sem_id, 0), "reserveSem");

    checkCall(shmdt(mat_ret), "shmdt");

    return 0;
}

int main(int argc, char *argv[])
{
    int shm_id, sem_id;
    
    shm_id = shmget(IPC_PRIVATE, sizeof(params), 0660 | IPC_CREAT);
    checkCall(shm_id, "shmget");

    sem_id = semget(IPC_PRIVATE, 2, 0660 | IPC_CREAT); 
    checkCall(sem_id, "semget");

    checkCall(initSemInUse(sem_id, 0), "initSemInUse"); 
    checkCall(initSemAvailable(sem_id, 1), "initSemAvailable");

    childPID = fork();

    switch(childPID)
    {
        case(-1):
            perror("fork");
            exit(EXIT_FAILURE);
        case(0): 
            child(shm_id, sem_id);
    }

    atexit(exitP);
    
    checkCall(parent(shm_id, sem_id), "parent");

    checkCall(semctl(sem_id, 0, IPC_RMID), "semctl");
    checkCall(shmctl(shm_id, IPC_RMID, NULL), "shmctl");

    exit(EXIT_SUCCESS);
}
