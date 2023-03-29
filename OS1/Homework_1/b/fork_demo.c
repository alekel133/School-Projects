/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC 12.2.0, Linux 6.0.10-arch2-1

    Description:
        Demo program showing the usage and error checking of the fork system
        call in C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

// Function for child to execute apon completion of fork()
int child() {
  for (int i = 1; i < 21; i++) {
    printf("I am the child: %d\n", i);
  }

  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

  // Catptures state of fork for error checking
  int stat = fork();

  // Checks the state of fork
  switch (stat) {
  case -1:
    // If fork failed, throw an erro and exit.
    perror("Error: fork()");
    exit(EXIT_FAILURE);

  case 0:
    // If fork was succesful, call child function
    child();
  }

  for (int i = 1; i < 21; i++) {
    printf("I am the parent: %d\n", i);
  }

  exit(EXIT_SUCCESS);
}