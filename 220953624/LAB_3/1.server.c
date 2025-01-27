// tcp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

void handle_client(int client_sock) {
    char buffer[MAX_BUF];
    int n;

    while (1) {
        // Read message from client
        memset(buffer, 0, MAX_BUF);
        n = read(client_sock, buffer, MAX_BUF);
        if (n <= 0) {
            printf("Client disconnected or error in reading\n");
            break;
        }
        printf("Client: %s", buffer);

        // Send response back to client
        printf("Server: ");
        memset(buffer, 0, MAX_BUF);
        fgets(buffer, MAX_BUF, stdin);
        write(client_sock, buffer, strlen(buffer));
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Fill server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in bind");
        close(server_sock);
        exit(1);
    }

    // Listen for incoming connections
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", PORT);

    client_len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Error in accept");
        close(server_sock);
        exit(1);
    }

    // Print PID and PPID
    printf("Server PID: %d, Server PPID: %d\n", getpid(), getppid());

    if (fork() == 0) {
        // Child process
        close(server_sock);
        handle_client(client_sock);
        close(client_sock);
        exit(0);
    }

    // Parent process
    close(client_sock);
    wait(NULL); // Wait for child process to finish
    close(server_sock);
    return 0;
}
