
#include "server.h"

char *directory = NULL;

/**
 * Get the request from the client and send a response
 * @param fd File descriptor of the client socket
 * @return void
 * @note mb for the conditional branching
 */
static
int get_request(int fd)
{
    char buffer[4096];
    char *token;

    int len = recv(fd, buffer, sizeof(buffer), 0);
    if (len < 0) {
        fprintf(stderr, "Error receiving data from client");
        return 84;
    }
    buffer[len] = '\0';
    token = strtok(buffer, " ");

    while (token != NULL) {
        if (strcmp(token, "GET") == 0) {
            token = strtok(NULL, " ");
            if (strcmp(token, "/") == 0) {
                send_200(fd);
                break;
            } else if (strncmp(token, "/echo/", 6) == 0) {
                send_echo(fd, token + 6);
                break;
            } else if (strcmp(token, "/user-agent") == 0) {
                send_agent(fd, token);
                break;
            } else if (strncmp(token, "/files/", 7) == 0) {
                send_file(fd, token + 7, directory);
                break;
            } else {
                send_404(fd);
                break;
            }
        }
        if (strcmp(token, "POST") == 0) {
            token = strtok(NULL, " ");
            if (strncmp(token, "/files/", 7) == 0) {
                send_post(fd, token + 7, directory);
                break;
            }
        }
        token = strtok(NULL, " ");
    }
    return 0;
}

/**
 * Handle the connection with the client (threaded)
 * @param arg File descriptor of the client socket
 * @return void
 */
void *handle_connection(void *arg) {
    int client_fd = *((int*)arg);

    free(arg);
    if (get_request(client_fd) == 84) {
        close(client_fd);
        return NULL;
    }
    close(client_fd);
    return NULL;
}

/**
 * Main function
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0
 */
int main(int argc, char **argv)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // Create a socket AF_INET for IPv4, SOCK_STREAM for TCP
    int client_fd;
    socklen_t client_addr_len;
    struct sockaddr_in server_addr; // Structure containing an internet address
    struct sockaddr_in client_addr;
    int *client_fd_ptr = NULL;
    pthread_t thread_id;
    int reuse = 1;

    if (server_fd == -1) {
        fprintf(stderr, "Error creating socket");
        return 84;
    }
    if (argc > 2) {
        if (strcmp(argv[1], "--directory") == 0) {
            directory = argv[2];
        }
    }

    /**
     * Set the SO_REUSEADDR option to allow the server to bind to an address that is already in use
    */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        printf("SO_REUSEADDR failed: %s \n", strerror(errno));
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) { // Bind (assign an address) to the socket
        fprintf(stderr, "Error binding socket");
        return 84;
    }
    if (listen(server_fd, 5) == -1) {
        fprintf(stderr, "Error listening on socket");
        return 84;
    }

    printf("Waiting for a client to connect...\n");

    while(1) {
        client_addr_len = sizeof(client_addr);
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            fprintf(stderr, "Error accepting connection");
            return 84;
        }
        printf("Client connected\n");

        client_fd_ptr = malloc(sizeof(int));
        *client_fd_ptr = client_fd;
        if(pthread_create(&thread_id, NULL, handle_connection, client_fd_ptr)
            != 0)
            fprintf(stderr, "Failed to create thread\n");
        pthread_detach(thread_id);
    }
    return 0;
}
