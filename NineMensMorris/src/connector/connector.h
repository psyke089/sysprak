#ifndef connector_h
#define connector_h

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>

#define MSGL 4096

/**
 * performConnection.c
 */

struct sockaddr_in init_server_addr();

int create_socket();

void connect_to_socket(int sock, struct sockaddr_in dest);

void get_message(int sock, char* buf);

void send_message(int sock, char* buf);

int performConnection(shm_struct *shm_str, plist_struct *plist_str, configData conf_str);

/**
 * parser.c
 */

void parseMessages(int sock, shm_struct *shm_str, plist_struct *plist_str, char *game_id, int *pipe_fd);

void processMessage(char *buf);

void tokenizeLine(char *line);

#endif