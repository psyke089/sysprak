#include <signal.h>
#include "../shm/shmManager.h"

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
 * sendet SIGUSR1
 *
 */
void start_thinking();


/**
 * startet den signal handler
 *
 */
void init_sig_action();

/**
 * returnt den signal status
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
 * setzt den think flag auf false
 * resetted die globale signal variable
 */
void write_to_pipe(int *fd, char *str, shm_struct* shm_str);

/**
 * die main KI methode
 *
 * gibt den zu Zug zurück
 */
char* think(plist_struct *plist_str);

