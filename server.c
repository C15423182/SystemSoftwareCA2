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
#include<pthread.h> //for threading , link with lpthread

#define PORT 9999
#define TRUE 1
#define FALSE 0
#define MAXCLIENTS 5

//connection handler thread
void *connection_handler( void *);


//main
int main (int argc, char *argv[])
{
    //server vars
    int socket_des, c_socket,connectionSize, readSize;
    // s = socket_des, cs = c_socket, connectionSize = connSize, readSize = readsize
    struct sockaddr_in  server, client;

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

    connectionSize = sizeof(struct  sockaddr_in);

    // c_socket = accept( socket_des, (struct socketaddr *)&client, (socklen_t*)&connectionSize);
    c_socket = accept(socket_des, (struct socketaddr *)&client, (socklen_t*)&connectionSize);

    if (c_socket < 0)
    {
        perror("Cant make connection\n");
        exit(1);
    }
    else
    {
        puts("Connetion from client accepted\n");
    }

    while(TRUE)
    {
        memset(msg, 0, 500);
        //readsize = read( c_socket, msg, 500);
        readSize = recv( c_socket, msg, 2000, 0);
        printf("From Client >> %s\n", msg);
        char msg_rec[] = "Recieved";
        int len_msg = strlen(msg_rec);
        write(c_socket, msg_rec, len_msg);

        if (strcmp(msg,"exit") == 0 || strcmp(msg,"EXIT") == 0  )
        {
            puts("Exiting Connection...\n");
            break;
        }
    }

    if ( readSize == 0)
    {
        puts("Client Disconnected");
        fflush(stdout);
    }
    else if (readSize == -1)
    {
        perror("Read Error");
    }

    puts("Server Closing...\n");
    close(socket_des);
    close(c_socket);
    exit(0);
}
