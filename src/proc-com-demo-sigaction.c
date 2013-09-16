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

/**
 * printf then flush stdout, too lazy to use write
 */
#define PFLUSH(...) {				\
    printf(__VA_ARGS__);			\
    fflush(stdout);				\
  }

/**
 * Alias for kill, because it's an awfully named function.
 */
#define SEND_SIG(pid, sig) kill(pid, sig)

/**
 * Just ridiculous
 */
#ifndef NAP_MAX
// max time to sleep between signal
#define NAP_MAX 5
#endif
#define TAKE_A_NAP sleep((rand() % NAP_MAX) + 1)
#define LUCKY rand() & 1

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
    struct sigaction user_1_action= {
      .sa_handler = &handle_user_signal,
      .sa_mask = 0,
      .sa_flags = NULL
    };
    signal(SIGUSR1, handle_user_signal);
    signal(SIGUSR2, handle_user_signal);
    signal(SIGINT, handle_interrupt);
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
void handle_user_signal(int sig) {
  PFLUSH("received: %s\n", strsignal(sig));
} 


/**
 * Handle Interrupt
 * Upon receiving a SIGINT, the parent will kill the child and exit.
 */
void handle_interrupt(int sig) {
  PFLUSH("\n\treceived: %s\n", strsignal(sig));
  PFLUSH("\tkilling child process...\n");
  SEND_SIG(child_pid, SIGKILL);
  PFLUSH("\tparent shutting down...\n");
  exit(0);
}


/**
 * Child Signal Loop
 * Waits 1-5 seconds then sends either a SIGUSR1 or SIGUSR2.
 */
void child_loop(void) {
  while(true) {
    TAKE_A_NAP;
    if (LUCKY) {
      SEND_SIG(parent_pid, SIGUSR1);
    } else {
      SEND_SIG(parent_pid, SIGUSR2);
    }
  }
} 
