#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAXLINE];
    int choice, n, number;

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(1);
    }

    while (1) {
        printf("\nEnter the number of elements in the set: ");
        scanf("%d", &n);

        printf("Enter the elements of the set: ");
        int arr[n];
        for (int i = 0; i < n; i++) {
            scanf("%d", &arr[i]);
        }

        printf("\nChoose an operation:\n");
        printf("1. Search for a number\n");
        printf("2. Sort the set (ascending/descending)\n");
        printf("3. Split the set into odd and even\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter the number to search for: ");
            scanf("%d", &number);
        }

        // Prepare the buffer to send to the server
        snprintf(buffer, sizeof(buffer), "%d %d", n, choice);
        for (int i = 0; i < n; i++) {
            snprintf(buffer + strlen(buffer) + 1 + i * 4, sizeof(buffer) - strlen(buffer) - 1, "%d", arr[i]);
        }

        // Send the data to the server
        send(sockfd, buffer, strlen(buffer), 0);

        // Receive the result from the server
        int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);
        buffer[bytes_received] = '\0';

        // Display the result
        printf("Server response: %s\n", buffer);

        // Exit condition
        if (choice == 4) {
            printf("Exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
