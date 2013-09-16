
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
#define TAKE_A_NAP sleep((rand() % 5) + 1)
#define LUCKY rand() & 1
#define ONE_TO_FIVE_SECS (rand() % 5) + 1

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
 * Prototypes
 * ************************************************************** */
void handle_signals(int);

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


