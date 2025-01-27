// udp_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUF 1024

void search_string(FILE *file, const char *search_str) {
    char line[MAX_BUF];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, search_str)) {
            count++;
        }
    }

    if (count > 0)
        printf("String found %d times.\n", count);
    else
        printf("String not found.\n");
}

void replace_string(FILE *file, const char *old_str, const char *new_str) {
    FILE *temp = fopen("temp.txt", "w");
    char line[MAX_BUF];
    
    while (fgets(line, sizeof(line), file)) {
        char *pos = strstr(line, old_str);
        if (pos != NULL) {
            *pos = '\0';
            fprintf(temp, "%s%s%s", line, new_str, pos + strlen(old_str));
        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(temp);
    remove("file.txt");
    rename("temp.txt", "file.txt");
    printf("String replaced.\n");
}

void reorder_file(FILE *file) {
    fseek(file, 0, SEEK_SET);
    char line[MAX_BUF];
    char lines[100][MAX_BUF];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        strcpy(lines[count], line);
        count++;
    }

    // Sort lines based on ASCII values of characters
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(lines[i], lines[j]) > 0) {
                char temp[MAX_BUF];
                strcpy(temp, lines[i]);
                strcpy(lines[i], lines[j]);
                strcpy(lines[j], temp);
            }
        }
    }

    // Write sorted content back to file
    FILE *sorted_file = fopen("sorted_file.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(sorted_file, "%s", lines[i]);
    }
    fclose(sorted_file);
    printf("File reordered.\n");
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[MAX_BUF];
    FILE *file;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server is waiting for connections...\n");

    client_len = sizeof(client_addr);
    
    while (1) {
        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        file = fopen(buffer, "r+");

        if (file == NULL) {
            sendto(server_fd, "File not present", strlen("File not present"), 0, (struct sockaddr *)&client_addr, client_len);
            continue;
        }

        sendto(server_fd, "File found", strlen("File found"), 0, (struct sockaddr *)&client_addr, client_len);

        recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        int choice = atoi(buffer);
        
        switch (choice) {
            case 1:
                recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
                search_string(file, buffer);
                break;
            case 2:
                recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
                char *str1 = buffer;
                recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
                char *str2 = buffer;
                replace_string(file, str1, str2);
                break;
            case 3:
                reorder_file(file);
                break;
            case 4:
                close(server_fd);
                return 0;
        }
    }

    close(server_fd);
    return 0;
}

