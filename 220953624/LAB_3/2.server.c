// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

void permute(char *str, int l, int r, int sockfd, struct sockaddr_in *cli_addr) {
    if (l == r) {
        sendto(sockfd, str, strlen(str), 0, (struct sockaddr *)cli_addr, sizeof(*cli_addr));
        return;
    }
    for (int i = l; i <= r; i++) {
        // Swap the characters
        char temp = str[l];
        str[l] = str[i];
        str[i] = temp;
        
        // Recur to generate permutations
        permute(str, l + 1, r, sockfd, cli_addr);
        
        // Backtrack
        temp = str[l];
        str[l] = str[i];
        str[i] = temp;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server is waiting for messages...\n");

    while (1) {
        // Receive message from client
        memset(buffer, 0, MAX_BUF);
        int n = recvfrom(sockfd, buffer, MAX_BUF, 0, (struct sockaddr *)&client_addr, &len);
        if (n < 0) {
            perror("Error in recvfrom");
            continue;
        }

        printf("Received message: %s\n", buffer);

        // Calculate and send permutations
        permute(buffer, 0, strlen(buffer) - 1, sockfd, &client_addr);
        printf("Sent all permutations back to client.\n");
    }

    close(sockfd);
    return 0;
}
