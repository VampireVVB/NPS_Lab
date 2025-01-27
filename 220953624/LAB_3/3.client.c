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
        perror("Error in connecting");
        close(sock);
        exit(1);
    }

    // Send message to server
    printf("Enter alphanumeric string: ");
    fgets(buffer, MAX_BUF, stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove trailing newline
    send(sock, buffer, strlen(buffer), 0);

    // Receive sorted numbers from the server
    memset(buffer, 0, MAX_BUF);
    int n = read(sock, buffer, MAX_BUF);
    if (n <= 0) {
        perror("Error in reading from server");
        close(sock);
        exit(1);
    }
    printf("Sorted numbers from server: %s\n", buffer);

    // Receive sorted characters from the server
    memset(buffer, 0, MAX_BUF);
    n = read(sock, buffer, MAX_BUF);
    if (n <= 0) {
        perror("Error in reading from server");
        close(sock);
        exit(1);
    }
    printf("Sorted characters from server: %s\n", buffer);

    // Close socket
    close(sock);
    return 0;
}
