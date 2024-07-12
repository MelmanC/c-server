
#include "server.h"

/**
 * Send a 200 OK response to the client
 * @param fd File descriptor of the client socket
 * @return void
 */
void send_200(int fd) {
	char *response = "HTTP/1.1 200 OK\r\n\r\n";
	send(fd, response, strlen(response), 0);
}

/**
 * Send a 404 Not Found response to the client
 * @param fd File descriptor of the client socket
 * @return void
 */
void send_404(int fd) {
	char *response = "HTTP/1.1 404 Not Found\r\n\r\n";
	send(fd, response, strlen(response), 0);
}

/**
 * Send an echo response to the client
 * @param fd File descriptor of the client socket
 * @param message Message to send
 * @return void
 */
void send_echo(int fd, char *message) {
	char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %d\r\n\r\n%s";
	char *buffer = malloc(strlen(response) + strlen(message) + 1);
	sprintf(buffer, response, strlen(message), message);
	send(fd, buffer, strlen(buffer), 0);
}

/**
 * Send the User-Agent (received) to the client
 * @param fd File descriptor of the client socket
 * @param token Token to send
 */
void send_agent(int fd, char *token) {
    while (token != NULL) {
        token = strtok(NULL, "\r\n");
        if (strncmp(token, "User-Agent:", 11) == 0) {
            token = token + 12;
			send_echo(fd, token);
            break;
        }
    }
}

/**
 * Send a file to the client
 * @param fd File descriptor of the client socket
 * @return void
 */
void send_file(int fd, char *token, char *directory) {
	FILE *file;
	struct stat st = {0};
	char *response = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: %d\r\n\r\n%s";
	char *buffer = NULL;
	char *buff = NULL;

	strcat(directory, token);
	stat(directory, &st);
    buff = malloc(sizeof(char) * st.st_size + 1);
    file = fopen(directory, "r");
	if (file == NULL) {
		send_404(fd);	
		return;
	}
    fread(buff, 1, st.st_size, file);
    buff[st.st_size] = '\0';
    fclose(file);
	buffer = malloc(strlen(response) + st.st_size + 1);
	sprintf(buffer, response, st.st_size, buff);
	send(fd, buffer, strlen(buffer), 0);
}

/**
 * Create a file with data received from the client
 * @param fd File descriptor of the client socket
 * @param token Token to send
 * @param directory Directory to save the file
 * @return void
 */
void send_post(int fd, char *token, char *directory) {
	FILE *file;
	char *response = "HTTP/1.1 201 Created\r\n\r\n";
	char *lastline = NULL;

	strcat(directory, token);
	file = fopen(directory, "w");
	if (file == NULL) {
		send_404(fd);
		return;
	}
	while (token != NULL) {
        lastline = token;
		token = strtok(NULL, "\r\n");
    }
	fwrite(lastline, 1, strlen(lastline), file);
	fclose(file);
	send(fd, response, strlen(response), 0);
}
