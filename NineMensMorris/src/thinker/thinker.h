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
 * Safe read
 *
 */
char* read_from_pipe(int *fd);

/**
 * Safe write
 *
 */
void write_to_pipe(int *fd, char *str);