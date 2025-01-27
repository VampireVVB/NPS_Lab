// udp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUF];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error in socket creation");
        exit(1);
    }

    // Fill server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter the string to send to the server: ");
    fgets(buffer, MAX_BUF, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

    // Send message to server
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Message sent to server: %s\n", buffer);

    // Receive all permutations from the server
    printf("Waiting for permutations...\n");
    while (1) {
        memset(buffer, 0, MAX_BUF);
        int n = recvfrom(sockfd, buffer, MAX_BUF, 0, NULL, NULL);
        if (n < 0) {
            perror("Error in recvfrom");
            break;
        }
        printf("Received permutation: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
