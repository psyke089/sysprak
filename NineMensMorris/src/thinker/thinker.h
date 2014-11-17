#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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
 * Methods
 * 
 */


/**
 * Key is IPC_PRIVATE 
 * Size is SHMSZ
 * 
 * returns the shm_id
 */
int create_shm();

/**
 * returns the attached struct
 *
 */

shm_struct* attach_shm(int shm_id);

/**
 * fills the struct with zeros
 */

void clear_shm(shm_struct *shm_s);

/**
 * sends IPC_RIMD - the segment it to delete 
 */

void delete_shm(int shm_id);

