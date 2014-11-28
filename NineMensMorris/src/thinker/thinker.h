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
 * an parent pid
 */
void start_thinking();

/**
 * startet den signal handler
 *
 * reagiert auf SIGUSR1 mit think()
 */
void init_sig_action();

/**
 * Safe read
 */
char* read_from_pipe(int *fd);

/**
 * Safe write
 *
 * setzt den think flag auf false
 */
void write_to_pipe(int *fd, char *str);

/**
 * die main KI methode
 *
 * prüft ob der think-flag im shm struct gesetzt wurde, sonst TERMINIERUNG
 * 
 * schreibt wenn er fertig mit der Berechnung ist den nächsten Zug
 * in die pipe mit der Größe von ANSWERLENGTH 
 */
void think();

