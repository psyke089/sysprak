#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>

#include "../main.h"
#include "../logger/logger.h"

#define MSGL 4096


/**
 * performConnection.c
 */

struct sockaddr_in init_server_addr();

int create_socket();

void connect_to_socket(int sock, struct sockaddr_in dest);

int get_message(int sock, char* buf);

void send_message(int sock, char* buf);

int setNonblocking(int sock);

void errMessageRoutine();

int performConnection();

/**
 * parser.c
 */

void parseMessages(int sock);

void parseSingleMessage(char *buf);
