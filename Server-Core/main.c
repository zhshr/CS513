#include <stdio.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include "shared.h"
#include "mysocket.h"

struct param{
    int portno;
};
void *core_thread_main(void *pParam);

int main(int argc, char* argv[]){
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];
    struct param p;
    pthread_t pth;
    //Determines the port number
    portno = getportno(argc, argv);
    p.portno = portno;
    pthread_create(&pth, NULL, core_thread_main, (void*)&p);
    pthread_join(pth, NULL);
    return 0;
}

void *core_thread_main(void *pParam){
    printf("Thread Started\n");

    //convert passed pointer into struct
    struct param p;
    p = *(struct param*)pParam;

    int sockfd;
    struct sockaddr_in cli_addr;

    sockfd = createsocket(p.portno,TRUE);

    printf("%d\n",sockfd);
    //clients
    int clients[MAX_CLIENTS];
    char (*nicknames[MAX_CLIENTS])[100];
    fd_set fds;

    //initialization of clients
    int i = 0;
    for (i = 0; i < MAX_CLIENTS; i++){
        clients[i] = 0;
    }

    //enter main loop

    int maxsd = 0;
    int activity = 0;
    int lenread = 0;
    char* buffer[BUFFER_SIZE];
    int new_socket, cli_addrlen;

    while (TRUE){
        //clear fd set
        FD_ZERO(&fds);

        //add parent socket to set
        FD_SET(sockfd, &fds);
        maxsd = sockfd;

        //add child sockets to set
        for (i = 0; i < MAX_CLIENTS; i++){
            //check valid
            if (clients[i] > 0){
                FD_SET(clients[i], &fds);
                if (clients[i] > maxsd)
                    maxsd = clients[i];
            }
        }
        activity = select(maxsd + 1, &fds, NULL, NULL, NULL);
        if ((activity < 0)){
            printf("ERROR: select error\n");
        }

        //check parent socket

        if (FD_ISSET(sockfd, &fds)){
            printf("Incoming connection\n");

            //accept incoming connection

 //           ;
            if ((new_socket = accept(sockfd, (struct sockaddr *)&cli_addr,
                    (socklen_t*)&cli_addrlen)) < 0){
                printf("ERROR: Accept connection failed\n");
            }else{

                printf("Connection established: socket %d, ip %s, port %d \n",
                        new_socket, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
            }

            //add the new socket into child array
            for (i = 0; i < MAX_CLIENTS; i++){
                if (clients[i] == 0){
                    clients[i] = new_socket;
                    free(nicknames[i]);
                    nicknames[i] = malloc(strlen("Annoymous")+1);
                    strcpy(nicknames[i], "Annoymous");
                    char temp[50];
                    sprintf(temp,"Hello, your client id is %d", i);
                    if (sendtosocket(new_socket, temp)==0){

                    }else{
                        printf("Hello sent\n");
                    }
                    break;
                }
            }
            printf("Socket Added, ID:%d\n", i);
        }

        //check other child sockets
        for (i = 0; i < MAX_CLIENTS; i++){
            if (clients[i] != 0){
                printf("Checking clients no.%d, fd %d\n", i, clients[i]);
                if (FD_ISSET(clients[i], &fds)){
                    printf("Message from socket number %d, fd %d\n", i, clients[i]);
                    //Socket No.i is ready to write
                    bzero(buffer, sizeof(buffer));
                    if ((lenread = read(clients[i], buffer, BUFFER_SIZE))==0){
                        // The socket is closed by client
                        getpeername(clients[i], (struct sockaddr*)&cli_addr, (socklen_t*)&cli_addrlen);
                        printf("Client disconnected: IP %s, port %d \n, nickname %s",
                                inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), nicknames[i]);

                        //close the socket
                        close(clients[i]);
                        clients[i] = 0;
                    }else{
                        //There is a message from the client
                        processmessage(buffer, i, &clients, &nicknames);
                    }
                }
            }
        }
    }
    printf("Thread Terminated\n");
    return NULL;
}

int getportno(int argc, char* argv[]){
    int portno = 0;
    if (argc<2){
        fprintf(stderr,"WARNING: No port provided. Using 12345 as default\n");
        portno = 11111;
    }else{
        portno = strtol(argv[1],NULL,10);
        if (errno!=0){
            fprintf(stderr,"WARNING: Port provided is invalid. Using 12345 as default\n");
            portno = 11111;
        }
    }
    printf("Using port %d\n",portno);
    return portno;
}




int setupmastersocket(int portno){

    //create parent socket
    //variables related with creating socket

}
