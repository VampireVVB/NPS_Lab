// tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUF];
    int n;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Fill server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        close(sock);
        exit(1);
    }

    // Print PID and PPID
    printf("Client PID: %d, Client PPID: %d\n", getpid(), getppid());

    while (1) {
        // Send message to server
        printf("Client: ");
        memset(buffer, 0, MAX_BUF);
        fgets(buffer, MAX_BUF, stdin);
        write(sock, buffer, strlen(buffer));

        // Receive message from server
        memset(buffer, 0, MAX_BUF);
        n = read(sock, buffer, MAX_BUF);
        if (n <= 0) {
            printf("Server disconnected or error in reading\n");
            break;
        }
        printf("Server: %s", buffer);
    }

    close(sock);
    return 0;
}
