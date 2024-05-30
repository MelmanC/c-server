#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <regex.h>

typedef struct server_s {
    int port;
    int socket;
} server_t;

void display_file(FILE *file, int sock, long length, char *content, char *response);

#endif /* SERVER_H_ */
