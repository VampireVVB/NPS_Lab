// udp_client.c
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

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter the file name: ");
    scanf("%s", file_name);

    sendto(sock, file_name, strlen(file_name), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
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
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (choice == 1) {
            printf("Enter string to search: ");
            scanf("%s", buffer);
            sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        } else if (choice == 2) {
            char str1[MAX_BUF], str2[MAX_BUF];
            printf("Enter string to replace: ");
            scanf("%s", str1);
            printf("Enter new string: ");
            scanf("%s", str2);
            sendto(sock, str1, strlen(str1), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
            sendto(sock, str2, strlen(str2), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        }

        if (choice == 4) {
            close(sock);
            break;
        }
    }

    return 0;
}

