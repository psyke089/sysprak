#define _GNU_SOURCE 

#include <signal.h>
#include <stdio.h>
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
 */
int write_to_pipe(int *fd, char *str);

/**
 * die main KI methode
 *
 * prüft ob der think-flag im shm struct gesetzt wurde, sonst TERMINIERUNG
 * 
 * schreibt wenn er fertig mit der Berechnung ist den nächsten Zug
 * in die pipe mit der Größe von ANSWERLENGTH 
 *
 * setzt den think flag am Ende der Berechnung auf false
 */
void calc_turn(shm_struct *shm_str, plist_struct *plist_str, int shm_id, int plist_id, int *fd);