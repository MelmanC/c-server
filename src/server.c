#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <regex.h>

void *handle_client(void *client_socket)
{
    int sock = *(int*)client_socket;
    char *buffer = malloc(sizeof(char) * 1024);
    long length = 0;
    char *content = NULL;
    FILE *file = NULL;
    char *response = NULL;
    char method[256];
    char url[256];

    read(sock, buffer, 1024);
    sscanf(buffer, "%s %s", method, url);
    if (strcmp(url, "/") == 0) {
        file = fopen("index.html", "r");
    } else if (strcmp(url, "/about") == 0) {
        file = fopen("about.html", "r");
    } else {
        file = fopen("404.html", "r");
    }

    display_file(file, sock, length, content, response);
    free(client_socket);
    free(buffer);
    free(response);
    return NULL;
}

/*
* This is the main function of the server
* It creates a socket, binds it to the localhost and port 8080
* Then it listens for incoming connections
* When a connection is accepted, it creates a new thread to handle the client
*/
int main(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;

    if (server_fd == -1)
        return 84;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
        return 84;
    if (listen(server_fd, 10) == -1)
        return 84;
    printf("Server is running on http://localhost:8080\n");

    while (1) {
        int *new_socket = malloc(sizeof(int));
        socklen_t addrlen = sizeof(address);
        if ((*new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen))>=0) {
            pthread_t client_thread;
            if (pthread_create(&client_thread, NULL, handle_client, new_socket) < 0) {
                return 84;
            }
        }
    }
    return 0;
}
