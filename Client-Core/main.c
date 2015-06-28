#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include "shared.h"
#include "mysocket.h"

void processmessage(char *message);
pthread_mutex_t socklock;

int main(int argc, char* argv[]){
    sockthread(processmessage);
}
int sockthread(void (*processor)(char*)){
    struct hostent *server;
    char name[100];
    strcpy(name,"127.0.0.1");
    server = gethostbyname(name);
    printf("%s %d %d %s\n", server->h_name, server->h_addrtype, server->h_length, server->h_addr);

    struct in_addr **addr_list;
    addr_list = (struct in_addr **)server->h_addr_list;
    printf("%s\n", inet_ntoa(*addr_list[0]));

    int sockfd;
    int portno = 2333;
    sockfd = createsocket(portno, 1000, FALSE);

    struct sockaddr_in serv_addr;
    int serv_portno = 11111;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(serv_portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        mylog("Error connecting to server");
        return EXIT_FAILURE;
    }else{
        mylog("Connected to server");
    }


    //ready to enter the main loop
    fd_set fds;
    int activity = 0;
    char buffer[BUFFER_SIZE];
    while (TRUE){
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        bzero(buffer, sizeof(buffer));

        activity = select(sockfd+1, &fds, NULL, NULL, NULL);
        if ((activity < 0)){
            mylog("ERROR: select error\n");
        }

        //check the connection with server
        if (FD_ISSET(sockfd, &fds)){
            if (read(sockfd, buffer, BUFFER_SIZE)==0){
                //server disconnected
                mylog("Server disconnected");
            }else{
                processor(buffer);
            }
        }
    }
}


void processmessage(char *message){
    char temp[BUFFER_SIZE];
    strcpy(temp, "Server returned:");
    strcat(temp, message);
    mylog(temp);
}
