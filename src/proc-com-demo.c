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

#define PFLUSH(...) {				\
		     printf(__VA_ARGS__);	\
		     fflush(stdout);		\
		     }

void handle_user_signal (int);
void handle_child_kill(int);
void handle_interrupt(int);
void child_loop(void);

pid_t child_pid;
pid_t parent_pid;
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

void handle_child_kill(int sig) {
  PFLUSH("child shutting down!\n");
}

void handle_user_signal(int sig) {
  PFLUSH("received: %s\n", strsignal(sig));
} 

void handle_interrupt(int sig) {
  PFLUSH("received: %s\n", strsignal(sig));
  PFLUSH("killing the child without remorse...\n");
  PFLUSH("parent shutting down...\n");
  exit(0);
}
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
