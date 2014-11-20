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
#define PLISTSZ (sizeof(plist_struct))

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
 * Field Struct
 * 
 * die Größe muss noch festgelegt werden
 * wahrscheinlich wieder hinreichend groß wählen
 * und eine Abfrage machen wie viel belegt wurde
 */
 typedef struct{
    char piece_list[2];
    int count;
 } plist_struct;


/**
 * Key ist IPC_PRIVATE
 * permissions sind 0666
 * 
 * gibt die shm_id zurück
 */
int create_shm(size_t size);

/**
 * Nur für Prozesse die nicht durch fork() entstanden sind
 * Magic number muss gesetzt werden
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
 * "befestigt" die Struktur an der shm_id
 * gibt die befestigte Struktur zurück
 */
plist_struct* attach_plist(int plist_id);

/**
 * füllt die Sturktur mit Nullen (0)
 */
void clear_shm(shm_struct *shm_s);

/**
 * füllt die Struktur mit Nullen (0)
 */
void clear_plist(plist_struct *plist_s);

/**
 * "entfernt" die Struktur
 */
void detach_shm(shm_struct *shm_s);

/**
 * "entfernt" die Struktur
 */
void detach_plist(plist_struct *plist_s);

/**
 * sendet IPC_RIMD nach der id
 * => das Segment wird zur Zerstörung freigegeben
 */
void delete_by_shmid(int shm_id);

/**
 * printet die Daten von der shm_struct
 * auf die Konsole aus
 */
void read_shm_struct(shm_struct* shm_str);

/**
 * füllt die struct mit Beispieldateien
 *
 * nur für Testzwecke
 *
 */
void fill_shm_struct(shm_struct* shm_str);


