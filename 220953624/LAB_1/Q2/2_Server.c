#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

// Function to check if a string is a palindrome
int is_palindrome(char str[]) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

// Function to count vowels in a string
void count_vowels(char str[], int *a, int *e, int *i, int *o, int *u) {
    *a = *e = *i = *o = *u = 0;
    for (int j = 0; str[j] != '\0'; j++) {
        char ch = tolower(str[j]);
        if (ch == 'a') (*a)++;
        else if (ch == 'e') (*e)++;
        else if (ch == 'i') (*i)++;
        else if (ch == 'o') (*o)++;
        else if (ch == 'u') (*u)++;
    }
}

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char response[MAXLINE];

    // Creating socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding the socket
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server is waiting for data...\n");

    while (1) {
        len = sizeof(client_addr);

        // Receiving data from client
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&client_addr, &len);
        buffer[n] = '\0';

        // If client sends "Halt", terminate the server
        if (strcmp(buffer, "Halt") == 0) {
            printf("Server terminating...\n");
            break;
        }

        // Check if the string is a palindrome
        int palindrome = is_palindrome(buffer);
        int length = strlen(buffer);

        // Count the occurrences of vowels
        int a, e, i, o, u;
        count_vowels(buffer, &a, &e, &i, &o, &u);

        // Prepare response message
        sprintf(response, "Palindrome: %s\nLength: %d\nOccurrences of vowels: A=%d, E=%d, I=%d, O=%d, U=%d",
                palindrome ? "Yes" : "No", length, a, e, i, o, u);

        // Send response to the client
        sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&client_addr, len);
    }

    // Close the socket
    close(sockfd);
    return 0;
}


