/*
Cian Sincliar
C15423182
System Software Assignment 2
Multithreaded Server / Client program
Server
*/
//imports
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h> //for threading , link with lpthread

#define PORT 9999
#define TRUE 1
#define FALSE 0
#define MAXCLIENTS 5

int connectionSize;
int socket_des;
struct sockaddr_in  server;
struct sockaddr_in client;

void AcceptMulClients();
void *ThreadClient(void *ptr);

//main
int main (int argc, char *argv[])
{
    //server vars
    int c_socket, readSize;
    // s = socket_des, cs = c_socket, connectionSize = connSize, readSize = readsize

    char msg[500];

    //create socket
    socket_des = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_des == -1)
    {
        puts("Socket not created\n");
        exit(1);
    }
    else
    {
        puts("Socket Created\n");
    }

    //prepare socket setup
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //bind
    if (bind(socket_des, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Bind Failed\n");
        exit(1);
    }
    else
    {
        printf("Bind Successful on port: %d\n", PORT);
    }

    //listen
    listen(socket_des, MAXCLIENTS);

    //accept connections
    puts("Waiting for connections.....\n");

    while(TRUE)
    {
        AcceptMulClients();
    }

    puts("Server Closing...\n");
    close(socket_des);
    close(c_socket);
    exit(0);
}

//function to accept multiple clients and set up sockets
void AcceptMulClients()
{
    int client_socket;

    listen(socket_des, MAXCLIENTS);
    connectionSize = sizeof(struct  sockaddr_in);

    client_socket = accept(socket_des, (struct socketaddr *)&client, (socklen_t*)&connectionSize);


    if (client_socket < 0)
    {
        perror("Cant make connection\n");
        exit(EXIT_FAILURE);
    }
    
    pthread_t client_thread;
    printf("Connection from Client %d accepted\n", client_socket);
    int *c = malloc(sizeof(*c));
    *c = client_socket;
    int threadReturn = pthread_create(&client_thread, NULL, ThreadClient, (void*) c);

    if(threadReturn)
    {
        printf("Failed to create client thread: %dn", threadReturn);
    }
}

//thread per client functionality 
void *ThreadClient(void *client_socket)
{
    int readSize;
    char msg[500];
    int CID = *((int*) client_socket);
    printf("Client %d\n", CID );
    memset(msg, 0, 500);

    while(TRUE)
    {
        readSize = recv( CID, msg, 2000, 0);
        printf("From Client >> %s\n", msg);
        char msg_rec[] = "Recieved";
        int len_msg = strlen(msg_rec);
        write(CID, msg_rec, len_msg);

        if (strcmp(msg,"exit") == 0 || strcmp(msg,"EXIT") == 0  )
        {
            printf("Exiting Connection...\n");
            exit(EXIT_SUCCESS);
        }
    }
}