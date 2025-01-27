// tcp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAX_BUF 1024

// Function to sort numbers in ascending order
void sort_numbers(char *str) {
    int numbers[100], count = 0;

    // Extract numbers from the string
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            numbers[count++] = str[i] - '0';
        }
    }

    // Sort numbers in ascending order
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (numbers[i] > numbers[j]) {
                int temp = numbers[i];
                numbers[i] = numbers[j];
                numbers[j] = temp;
            }
        }
    }

    // Place the sorted numbers back in the string
    count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            str[i] = numbers[count++] + '0';
        }
    }
}

// Function to sort characters in descending order
void sort_characters(char *str) {
    int length = strlen(str);

    // Sort characters in descending order using bubble sort
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            if (isalpha(str[i]) && isalpha(str[j]) && str[i] < str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[MAX_BUF];
    pid_t pid;

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
        perror("Error in binding");
        close(server_sock);
        exit(1);
    }

    // Listen for incoming connections
    listen(server_sock, 5);
    printf("Server is listening on port %d...\n", PORT);

    client_len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Error in accepting connection");
        close(server_sock);
        exit(1);
    }

    printf("Client connected.\n");

    // Receive message from client
    memset(buffer, 0, MAX_BUF);
    int n = read(client_sock, buffer, MAX_BUF);
    if (n <= 0) {
        perror("Error in reading from client");
        close(client_sock);
        close(server_sock);
        exit(1);
    }
    printf("Received message from client: %s\n", buffer);

    // Fork a child process to sort numbers
    if ((pid = fork()) == 0) {
        // Child process: sort numbers in ascending order
        sort_numbers(buffer);
        printf("Child (PID: %d) sorted numbers: %s\n", getpid(), buffer);
        write(client_sock, buffer, strlen(buffer));  // Send result to client
        close(client_sock);
        exit(0);
    }

    // Parent process: sort characters in descending order
    sort_characters(buffer);
    printf("Parent (PID: %d) sorted characters: %s\n", getpid(), buffer);
    write(client_sock, buffer, strlen(buffer));  // Send result to client

    // Close sockets
    close(client_sock);
    close(server_sock);

    return 0;
}
