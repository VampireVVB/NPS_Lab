#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in server_addr;
    socklen_t len;

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(server_addr);

    while (1) {
        printf("Enter a string (or 'Halt' to quit): ");
        fgets(buffer, MAXLINE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // Remove trailing newline

        // Send the string to the server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_addr, len);

        // If the user enters "Halt", terminate the client
        if (strcmp(buffer, "Halt") == 0) {
            printf("Client terminating...\n");
            break;
        }

        // Receive the result from the server
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&server_addr, &len);
        buffer[n] = '\0';

        // Display the received result
        printf("Server response:\n%s\n", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}
