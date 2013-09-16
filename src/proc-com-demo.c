#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

#include "proc-com-demo.h"

/* **************************************************************
 * Code
 * ************************************************************** */

/**
 * Main
 */
int main(int argc, char* argv[], char* envp[]) {
  parent_pid = getpid();
  child_pid = fork();
  switch(child_pid) {
  case -1: {
    PFLUSH("Fork failed!\n");
    exit(-1);
  }
  case 0: {
    child_loop();
    break;
  }
  default: {
    signal(SIGUSR1, handle_signals);
    signal(SIGUSR2, handle_signals);
    signal(SIGINT, handle_signals);
    while(1) {
      PFLUSH("waiting...         ");
      pause();
    }
    break;
  }
  }
  return 0;
}

void handle_signals(int sig) {
  switch(sig) {
  case SIGUSR1:
  case SIGUSR2: {
    PFLUSH("received: %s\n", strsignal(sig));
    break;
  }
  case SIGINT: {
    PFLUSH("\n\treceived: %s\n", strsignal(sig));
    PFLUSH("\tkilling child process...\n");
    SEND_SIG(child_pid, SIGKILL);
    PFLUSH("\tparent shutting down...\n");
    exit(0);
    break;
  }
  } 
} 
