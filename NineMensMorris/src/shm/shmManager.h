#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "../main.h"

/**
 * Config
 */

#define NAMELENGTH 100
#define MAXPLAYERS 8
#define SHMSZ (sizeof(shm_struct))

/**
 * Player Struct
 *
 */
typedef struct{
  int playerID;
  char playerName[NAMELENGTH];
  bool isReady;
  bool isLoggedIn;
} player_struct;


/**
 * Shared Memory Struct
 *
 */
 typedef struct{
  char gameName[NAMELENGTH];
  char gameID[11];
  int playerCount;
  player_struct p_structs[MAXPLAYERS];
  int p_pid;
  int c_pid;
} shm_struct;


/**
 * Key ist IPC_PRIVATE (gerade eine magic number)
 * Size ist SHMSZ
 * permissions sind 0666
 *
 * gibt die shm_id zurück
 */
int create_shm();

/**
 * Key ist IPC_PRIVATE (gerade eine magic number)
 * Size ist SHMSZ
 * 
 * gibt die shm_id zurück
 */
int locate_shm();

/**
 * "befestigt" die Struktur an der shm_id 
 * gibt die befestigte Struktur zurück
 */
shm_struct* attach_shm(int shm_id);

/**
 * füllt die Sturktur mit Nullen (0)
 */
void clear_shm(shm_struct *shm_s);

/**
 * "entfernt" die Struktur
 */
void detach_shm(shm_struct *shm_s);

/**
 * sendet IPC_RIMD nach der id
 * => das Segment wird zur Zerstörung freigegeben
 */
void delete_shm(int shm_id);

/**
 * printet die Daten von der shm_struct
 * auf die Konsole aus
 *
 **/
void read_shm_struct(shm_struct* shm_str);

/**
 * füllt die struct mit Beispieldateien
 *
 * nur für Testzwecke
 *
 */
void fill_shm_struct(shm_struct* shm_str);