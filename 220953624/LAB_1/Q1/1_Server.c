#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

// Function to search for a number in the set
int search_number(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key) {
            return i;
        }
    }
    return -1; // Not found
}

// Function to sort the array in ascending order
void sort_ascending(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// Function to sort the array in descending order
void sort_descending(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (arr[i] < arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// Function to split the array into odd and even numbers
void split_odd_even(int arr[], int n, int *odd, int *even, int *odd_size, int *even_size) {
    *odd_size = 0;
    *even_size = 0;
    
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            even[*even_size] = arr[i];
            (*even_size)++;
        } else {
            odd[*odd_size] = arr[i];
            (*odd_size)++;
        }
    }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[MAXLINE];
    int arr[MAXLINE], odd[MAXLINE], even[MAXLINE];
    int choice, number, n, odd_size, even_size;
    ssize_t bytes_received;

    // Create the socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == -1) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept client connection
    len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len)) == -1) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    while (1) {
        // Receive the set of integers and choice from the client
        bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        buffer[bytes_received] = '\0';

        // Parse the received data
        sscanf(buffer, "%d %d", &n, &choice);
        for (int i = 0; i < n; i++) {
            sscanf(buffer + strlen(buffer) + 1 + i * 4, "%d", &arr[i]);
        }

        // Perform the relevant operation based on the choice
        char result[MAXLINE] = "";

        if (choice == 1) {
            // Search for a number
            printf("Enter the number to search: ");
            scanf("%d", &number);
            int index = search_number(arr, n, number);
            if (index != -1) {
                sprintf(result, "Number found at index %d", index);
            } else {
                sprintf(result, "Number not found");
            }
        } else if (choice == 2) {
            // Sort the array
            int order;
            printf("Enter 1 for ascending, 2 for descending: ");
            scanf("%d", &order);
            if (order == 1) {
                sort_ascending(arr, n);
                sprintf(result, "Sorted in ascending order: ");
            } else {
                sort_descending(arr, n);
                sprintf(result, "Sorted in descending order: ");
            }
            for (int i = 0; i < n; i++) {
                sprintf(result + strlen(result), "%d ", arr[i]);
            }
        } else if (choice == 3) {
            // Split into odd and even
            split_odd_even(arr, n, odd, even, &odd_size, &even_size);
            sprintf(result, "Odd numbers: ");
            for (int i = 0; i < odd_size; i++) {
                sprintf(result + strlen(result), "%d ", odd[i]);
            }
            sprintf(result + strlen(result), "\nEven numbers: ");
            for (int i = 0; i < even_size; i++) {
                sprintf(result + strlen(result), "%d ", even[i]);
            }
        } else if (choice == 4) {
            // Exit
            printf("Server exiting...\n");
            break;
        }

        // Send the result back to the client
        send(client_fd, result, strlen(result), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
