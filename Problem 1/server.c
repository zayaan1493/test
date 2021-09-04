
/*
Name : server.c
Creator : Md Laadla (mailzayaan1493.ml@kgpian.iitkgp.ac.in)
Roll No : 20CS60R20
Date : 12th January 2021
Description : This is the server side code for simple message passing to clients one by one. The server after getting connected to 
the client waits to accept a message from the client and then acknowledges this message by sending an acknowledgment to the client
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define MAX 1000 // max length of the sent and recieved strings between server and client

int main(int argc, char **argv)
{
	int port;
	sscanf(argv[1], "%d", &port);// the port taken from command line
	int serv,new_sock;//socket descriptor for the server

	char in[MAX]={0},*out;
	
	out=(char *)malloc(50*sizeof(char));
	out="Acknowledged by the Server!!!\n";


	serv=socket(AF_INET, SOCK_STREAM, 0);
	// AF_NET --> Ipv4 protocol, SOCK_STREAM --> TCP, 0-->IP
	if(serv<=0){
		printf("Error while creating socket in Server");
		exit(0);
	}



	printf("Socket Created!\n");
	struct sockaddr_in serv_addr;

	bzero((char *)&serv_addr,sizeof(serv_addr));
	// initializes with zero

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;//Kernel chooses the addresses
	serv_addr.sin_port = htons(port);//converts the host byte order to network byte order

	int b = bind(serv, (struct sockaddr *)& serv_addr, sizeof(serv_addr));// binding IP address and port number to create a socket

	if (b<0)
	{
		printf("Binding Error from Server\n");
		exit(0);
	}

	printf("Bind done\n");

	int l = listen(serv,3);// Listening for clients to connect, total 3 pending clients it can accomodate in the queue

	if (l<0)
	{
		printf("Error while listening by the Server");
		exit(0);
	}

	printf("Starting to accept\n");


	while(1)
	{
		int len = sizeof(serv_addr);
		new_sock = accept(serv, (struct sockaddr *)& serv_addr,  (socklen_t *)&len);// accepts and returns the socket descriptor to which the client communicates with the server

		if (new_sock<0)
		{
			printf("Error while accepting the connection by the server");
			exit(0);
		}

		printf("Accepted!\n");

		int r = read(new_sock, in, MAX);//reads from the client


		if (r<0)
		{
			printf("Error while reading by the Server");
			exit(0);
		}

		printf("%s\n",in);
		send(new_sock, out, strlen(out), 0);//sent acknowledgement to the client

		close(new_sock);


	}
	

	close(serv);



	return 0;
}