#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <regex.h>

void display_file(FILE *file, int sock, long length, char *content, char *response)
{
    if (file != NULL) {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        content = malloc(length);
        if (content) {
            fread(content, 1, length, file);
        }
        fclose(file);
        response = malloc(strlen("HTTP/1.1 200 OK\nContent-Type: text/html\n\n") + length + 1);
        strcpy(response, "HTTP/1.1 200 OK\nContent-Type: text/html\n\n");
        strcat(response, content);
        free(content);
    } else {
        response = malloc(strlen("HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n") + 1);
        strcpy(response, "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n");
    }
    send(sock, response, strlen(response), 0);
    close(sock);
}
