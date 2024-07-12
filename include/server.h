#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

// send data
void send_200(int fd);
void send_404(int fd);
void send_echo(int fd, char *message);
void send_agent(int fd, char *token);
void send_file(int fd, char *token, char *directory);
void send_post(int fd, char *token, char *directory);


#endif /* SERVER_H_ */
