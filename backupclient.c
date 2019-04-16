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

//main
int main (int argc, char *argv[])
{
	int SID;
	struct sockaddr_in  server;
	char client_msg[500], server_msg[500];

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
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9999);

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


    while(1)
    {
    	printf("Client >> ");
    	scanf("%s", client_msg);
    	int msg_size = strlen(client_msg);

    	if (client_msg == "exit" || client_msg == "EXIT")
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
    		puts("Recieve Error/nBreaking Connection");
    		break;
    	}

    	printf("Server >>  %s", server_msg);
    }

    close(SID);
    exit(0);
}