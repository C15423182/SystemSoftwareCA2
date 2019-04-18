/*
Cian Sincliar
C15423182
System Software Assignment 2
Multithreaded Server / Client program
Client
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

#define IP "127.0.0.1"
#define PORT 9999
#define TRUE 1
#define FALSE 0

int SID;
char FILE_SOURCE[200];
char DIR_DEST[200];
char client_msg[500];
char server_msg[500];
struct sockaddr_in  server;

//main
int main (int argc, char *argv[])
{
    strcpy(FILE_SOURCE, argv[1]);
    strcpy(DIR_DEST, argv[2]);

    int socket_fd;

    if (strcmp(DIR_DEST,"Sales") == 0 || strcmp(DIR_DEST,"Promotions") == 0 || 
        strcmp(DIR_DEST,"Offers") == 0 || strcmp(DIR_DEST,"Root") == 0 || strcmp(DIR_DEST,"Sales") == 0)
    {
        printf("Accept\n");
    }
    else
    {
        printf("failed\n");
    }

    SID = SocketCreate();

    while(TRUE)
    {
        printf("To Server >> ");
        scanf("%s", client_msg);
        int msg_size = strlen(client_msg);

        if (strcmp(client_msg,"exit") == 0 || strcmp(client_msg,"EXIT") == 0 )
        {
            puts("Exiting Connection\n");
            exit(0);
            break;
        }

        //send msg
        if ( send(SID, client_msg, msg_size, 0) < 0)
        {
            puts("Sending Failed\n");
            exit(1);
        }

        //recieve msg
        if ( recv( SID, server_msg, 500, 0) < 0)
        {
            puts("Recieve Error\nBreaking Connection");
            break;
        }
    }

    close(SID);
    exit(0);
}

int SocketCreate()
{
    SID = socket(AF_INET, SOCK_STREAM, 0);

    if (SID == -1)
    {
        puts("Error Client Socket Creation\n");
        exit(1);
    }
    else
    {
        puts("Client Socket Creation Complete\n");
    }

    //prepare socket setup
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);

    if (connect(SID, (struct socketaddr *)&server, sizeof(server)) < 0)
    {
        puts("Connection failed");
        exit(1);
    }
    else
    {
        puts("Connection made to server");
        puts("Enter \"exit\" to exit connection\n");
    }

    return SID;
}