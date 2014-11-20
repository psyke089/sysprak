#include <signal.h>
#include "../main.h"

/**
 * Config
 */
#define ANSWERLENGTH 3

/**
 * Pipehelper
 */
#define READ 0
#define WRITE 1



/**
 * sends SIGUSR1
 *
 */
void start_thinking();


/**
 * starts the signal handler
 *
 */
void init_sig_action();


/**
 * returns the signal status
 *
 */
sig_atomic_t get_signal();

/**
 * Safe read
 *
 */
char* read_from_pipe(int *fd);

/**
 * Safe write
 *
 */
void write_to_pipe(int *fd, char *str);


