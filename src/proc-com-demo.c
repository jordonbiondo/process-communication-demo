#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char* argv[], char* envp[]) {
  printf("Hello!");
  return 0;
}

