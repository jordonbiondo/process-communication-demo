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
 * Pid of second child.
 */
pid_t child_pid2;

/**
 * 
 */
void handle_signals_info(int, siginfo_t*, void*);

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
    child_entry:
    child_loop();
    break;
  }
  default: {
    child_pid2 = fork();
    if (!child_pid2) {
      goto child_entry; 
    }
    struct sigaction sig_action;
    sig_action.sa_sigaction = &handle_signals_info;
    sig_action.sa_flags = SA_SIGINFO;
    sigemptyset(&sig_action.sa_mask);
    sigaction(SIGUSR1, &sig_action, NULL);
    sigaction(SIGUSR2, &sig_action, NULL);
    sigaction(SIGINT, &sig_action, NULL);
    while(1) {
      PFLUSH("waiting...         ");
      pause();
    }
    break;
  }
  }
  return 0;
}


/**
 * Handle User Signal.
 * Log the occurence of receiving a SIGUSR1 or SIGUSR2.
 */
void handle_signals_info(int sig, siginfo_t* info, void* v) {
  switch(sig) {
  case SIGUSR1:
  case SIGUSR2: {
    PFLUSH("received %s from: %d\n", strsignal(sig), info->si_pid);
    break;
  }
  case SIGINT: {
    PFLUSH("\n\treceived: %s\n\t%s %d... %s\n\t%s %d... %s\n\tparent shutting down...\n", 
	   strsignal(sig),
	   "killing child", child_pid,  (SEND_SIG(child_pid, SIGKILL) == 0) ? "OK" : "FAIL",
	   "killing child", child_pid2, (SEND_SIG(child_pid2, SIGKILL) == 0) ? "OK" : "FAIL");
    exit(0);
    break;
  }
  } 

}
