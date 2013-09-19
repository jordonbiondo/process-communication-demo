
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

//max number of seconds for child to sleep -- see dbg build
#ifndef NAP_MAX
#define NAP_MAX 5 
#endif

/**
 * Just ridiculous
 */
#define TAKE_A_NAP sleep((rand() % NAP_MAX) + 1)
#define LUCKY rand() & 1
#define FOREVER while(true)
#define SOMETIMES(this, skipme, that) {					\
    if (LUCKY) this; else that;						\
  }					

  
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
 * Waits 1-NAP_MAX seconds then sends either a SIGUSR1 or SIGUSR2.
 */
void child_loop(void) {
  FOREVER {
    TAKE_A_NAP;
    SOMETIMES(SEND_SIG(parent_pid, SIGUSR1), OTHERTIMES, SEND_SIG(parent_pid, SIGUSR2));
	      
  }
} 


