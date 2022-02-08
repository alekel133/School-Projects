/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
        Creates a basic directory structure. 
*/  
#include "hdrs.h"

// Checks if a function has failed, if so, exits program.
// *Taken from Prof. Aubrey Knigh, Systems Programming
// Week 3, Lecture 2
void checkCall(int val, const char *msg)
{
    if(val == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

// Creates a given amount of directories with a common naming
// scheme, i.e. "exa1, exa2, exa3..."
void fillDir(int range, mode_t perms, const char *reg)
{
    int i;
    char dirname[1024];
    for(i = 0; i < range; i++)
    {
        sprintf(dirname, "%s%d",reg, i + 1);
        checkCall(mkdir(dirname,perms), "mkdir");
    }
}

int main(int argc, char *argv[])
{
    char dirname[1024] = "sysProg";
    int i = 0; 
    mode_t perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP;
    
    // Checks to see if proper arguments have been provided
    if(argc != 1 && argc != 2)
    {
        perror("Syntax creatClass <dirname>");
        exit(EXIT_FAILURE);
    }

    // If there is an arguement, use it as the name for root
    // directory
    if(argc == 2)
    {
        sscanf(argv[1], "%s", dirname);
    }

    // Makes the root directory, and first three subdirectories
    checkCall(mkdir(dirname, perms), "mkdir");
    checkCall(chdir(dirname), "chdir");
    checkCall(mkdir("test", perms), "mkdir"); 
    checkCall(mkdir("program", perms), "mkdir");
    checkCall(mkdir("exercise", perms), "mkdir");

    // Fills the test directory
    checkCall(chdir("test"), "chdir");
    fillDir(3, perms, "test");
    checkCall(chdir(".."), "chdir");
    
    //FIlls the program directory
    checkCall(chdir("program"), "chdir");
    fillDir(5, perms, "prog");
    checkCall(chdir(".."), "chdir");

    //Fills the exercise directory
    checkCall(chdir("exercise"), "chdir");
    fillDir(10, perms, "exer");
    checkCall(chdir(".."), "chdir");

    checkCall(chdir(".."), "chdir");

    exit(EXIT_SUCCESS);
}
