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


/* **************************************************************
 * Macros
 * ************************************************************** */

#define PFLUSH(...) {				\
		     printf(__VA_ARGS__);	\
		     fflush(stdout);		\
		     }

/* **************************************************************
 * Prototypes
 * ************************************************************** */

void handle_user_signal (int);


void handle_interrupt(int);


void child_loop(void);

/* **************************************************************
 * Globals
 * ************************************************************** */

/**
 * Child Process ID.
 */
pid_t child_pid;


/**
 * Parent Process ID.
 */
pid_t parent_pid;

/* **************************************************************
 * Code
 * ************************************************************** */

/**
 * Main
 */
int main(int argc, char* argv[], char* envp[]) {
  parent_pid = getpid();
  child_pid = fork();
  if (child_pid) {
    signal (SIGUSR1, handle_user_signal);
    signal (SIGUSR2, handle_user_signal);
    signal (SIGINT, handle_interrupt);
    while(1) {
      PFLUSH("waiting...         ");
      pause();
    }
  } else {
    child_loop();
  }
  return 0;
}


/**
 * Handle User Signal.
 * Log the occurence of receiving a SIGUSR1 or SIGUSR2.
 */
void handle_user_signal(int sig) {
  PFLUSH("received: %s\n", strsignal(sig));
} 


/**
 * Handle Interrupt
 * Upon receiving a SIGINT, the parent will kill the child and exit.
 */
void handle_interrupt(int sig) {
  PFLUSH("received: %s\n", strsignal(sig));
  PFLUSH("killing the child without remorse...\n");
  kill(child_pid, SIGKILL);
  PFLUSH("parent shutting down...\n");
  exit(0);
}


/**
 * Child Signal Loop
 * Waits 1-5 seconds then sends either a SIGUSR1 or SIGUSR2.
 */
void child_loop(void) {
  while(true) {
    sleep((rand() % 5) + 1);
    if (rand() & 1) {
      /* raise(SIGUSR1); */
      kill(parent_pid, SIGUSR1);
    } else {
      /* raise(SIGUSR2); */
      kill(parent_pid, SIGUSR2);
    }
  }
}
