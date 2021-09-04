Name : Md Laadla
Roll no : 20CS60R20




Program 1 :

Server compile command : gcc server.c -o server.o
Server run command : ./server.o 8080

Client compile command : gcc client.c -o client.o
Client run command : ./client.o localhost 8080



Program 2 :


Server compile command : gcc server.c -o server.o -lm
Server run command : ./server.o 8080

Client compile command : gcc client.c -o client.o
Client run command : ./client.o localhost 8080


NOTE : IN PROGRAM 2 IN SERVER COMPILE COMMAND LINK MODULE (-lm) IS USED SINCE math.h IS IMPORTED.