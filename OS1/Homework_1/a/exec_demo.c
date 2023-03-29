/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC 12.2.0, Linux 6.0.10-arch2-1

    Description:
        Demo program showing the usage, behavior, and error checking of the
        exec system call in C.
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Argument array
  char *mycmd[4];

  // Command line syntax checking
  if (argc > 2) {
    perror("Syntax Error: ./program (optional directory)");
    exit(EXIT_FAILURE);
  }

  // Loading argument array
  mycmd[0] = "ls\0";
  mycmd[1] = "-l\0";
  mycmd[3] = NULL;

  // Dynamic argument implementation
  if (argc == 2) {
    mycmd[2] = argv[1];
  } else {
    mycmd[2] = "/usr\0";
  }

  // Exec(str file, str args[]) call
  execvp("ls", mycmd);

  // Prints error message if exec fails (success results in exiting from the
  // executed program).
  perror("EXEC Error: ");
  exit(EXIT_FAILURE);
}