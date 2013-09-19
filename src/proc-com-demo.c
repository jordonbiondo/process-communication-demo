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
#include "external-strings.h"

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
    PFLUSH("%s\n", rs$(failed_fork));
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
      PFLUSH("%s", rs$(waiting));
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
    PFLUSH("%s: %s\n", rs$(received), strsignal(sig));
    break;
  }
  case SIGINT: {
    PFLUSH("\n\t%s: %s\n\t%s %d... %s\n\t%s...\n", 
	   rs$(received),
	   strsignal(sig),
	   rs$(killing_child),
	   child_pid, 
	   ((SEND_SIG(child_pid, SIGKILL) == 0) ? rs$(ok) : rs$(fail)),
	   rs$(parent_shutdown)
	   );
    exit(0);
    break;
  }
  } 
} 
