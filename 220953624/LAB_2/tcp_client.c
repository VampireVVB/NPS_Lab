// tcp_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUF];
    char file_name[MAX_BUF];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Get file name from user
    printf("Enter the file name: ");
    scanf("%s", file_name);
    send(sock, file_name, strlen(file_name), 0);

    // Receive server response
    recv(sock, buffer, sizeof(buffer), 0);
    if (strcmp(buffer, "File not present") == 0) {
        printf("File not present\n");
        close(sock);
        return 0;
    } else {
        printf("File found.\n");
    }

    // Menu
    while (1) {
        printf("\n1. Search\n2. Replace\n3. Reorder\n4. Exit\n");
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);
        sprintf(buffer, "%d", choice);
        send(sock, buffer, strlen(buffer), 0);

        if (choice == 1) {
            printf("Enter string to search: ");
            scanf("%s", buffer);
            send(sock, buffer, strlen(buffer), 0);
        } else if (choice == 2) {
            char str1[MAX_BUF], str2[MAX_BUF];
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter new string: ");
            scanf("%s", str2);
            send(sock, str1, strlen(str1), 0);
            send(sock, str2, strlen(str2), 0);
        }

        // Exit option
        if (choice == 4) {
            close(sock);
            break;
        }
    }

    return 0;
}
