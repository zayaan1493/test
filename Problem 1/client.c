/*
Name : client.c
Creator : Md Laadla (mailzayaan1493.ml@kgpian.iitkgp.ac.in)
Roll No : 20CS60R20
Date : 12th January 2021
Description : This code just checks the connection between the client and server. This is client's code in which the client connects
to the server by sending some arbiratory message and then accepts server's acknowlegment and then the client ends.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define MAX 1000 // max length of the sent and recieved strings between server and client

int main(int argc, char** argv)
{
	int port;

	sscanf(argv[2], "%d", &port);// The port at which the server and client connects

	int cli;// socket descriptor at the client side

	char in[MAX]={0};//in--> to read the results sent by the server
	char out[MAX]={0};//out --> the string containing the message to be sent to the server
	
	sprintf(out,"%s","Arbiratariy message from the client!!");
	

	cli = socket(AF_INET, SOCK_STREAM, 0);

	if (cli<0)
	{
		printf("Socket Creation Problem by Client\n");
		exit(0);
	}

	printf("Socket Created!\n");

	struct sockaddr_in serve_addr;// the socket address

	struct hostent *server;

	bzero((char *)&serve_addr,sizeof(serve_addr));//Initializes buffer

	serve_addr.sin_family = AF_INET;

	server = gethostbyname(argv[1]);//host address to IP address

	bcopy((char *)server->h_addr, (char *)&serve_addr.sin_addr.s_addr,server->h_length);//copy server IP address
	
	serve_addr.sin_port = htons(port);// defining the port address


	int con = connect(cli, (struct sockaddr *)&serve_addr, sizeof(serve_addr));// connect request to the server

	if (con<0)
	{
		printf("Error while connecting from the Client\n");
		exit(0);
	}
	printf("Connected ...\n");

	printf("Sending from client\n");
	send(cli, out, strlen(out),0);//send data to the server
	int r = read(cli, in, 50);// read data from the server

	if (r<0)
	{
		printf("Error while reading by the client\n");
		exit(0);
	}
	printf("%s\n",in);
	
	close(cli);


	return 0;
}