#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8090

int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char ser_ip[16];

void broadcast(char *message)
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (clients[i] != -1)
        {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *listen_messages(void *arg)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "start", strlen("start"))==0)
        {
            char res[50];
            strcpy(res,"./runner.sh ");
            strcat(res, ser_ip);
            broadcast(res);
        }
        else if (strncmp(buffer, "stop", strlen("stop"))==0)
            broadcast("./stopper.sh");
        else if (strncmp(buffer, "stat", strlen("stat"))==0)
            broadcast("./status.sh");
        else if (strncmp(buffer, "make", strlen("make"))==0)
            broadcast("./maker.sh");
        else
            broadcast(buffer);
    }
    return NULL;
}

int main()
{
    int server_socket, client_socket, addr_size;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;

    // Initialize client array
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        clients[i] = -1;
    }

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server address to socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Get server socket address information
    struct sockaddr_in temp_addr;
    socklen_t temp_len = sizeof(temp_addr);
    getsockname(server_socket, (struct sockaddr *)&temp_addr, &temp_len);

    // Print server IP and port
    printf("Server started. Listening on %s:%d\n", inet_ntoa(temp_addr.sin_addr), ntohs(temp_addr.sin_port));
    system("ifconfig | grep 'inet ' | grep -v '127.0.0.1' | awk '{print $2}' > ssn_ip.txt");

    FILE *fp = fopen("ssn_ip.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read the content of the file
    while (fgets(ser_ip, sizeof(ser_ip), fp) != NULL)
    {
        printf("%s", ser_ip);
    }

    // Close the file
    fclose(fp);

    remove("ssn_ip.txt");

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Create thread for listening to messages from the server
    if (pthread_create(&tid, NULL, listen_messages, NULL) != 0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        addr_size = sizeof(struct sockaddr_in);

        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_size);
        if (client_socket < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Add client to array
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if (clients[i] == -1)
            {
                clients[i] = client_socket;
                client_count++;
                break;
            }
        }

        printf("Client %d joined.\n", client_socket);
    }

    close(server_socket);

    return 0;
}
