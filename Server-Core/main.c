#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "main.h"

#define TRUE 1
#define FALSE 0

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

    //variables related with creating socket
    int opt = TRUE;
    int sockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;

    //set port number and bind port
    portno = p.portno;

    //create parent socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        printf("ERROR: Failed creating socket.");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket File Descripter: %d\n", sockfd);
    }

    //set socket to accept multiple connections
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        printf("ERROR: Set Reuse Addr failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Set Reuse Addr successful\n");
    }

    //bind port
    serv_addr = getservaddr(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("ERROR: Port binding failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Port binding successfully, listening on port %d\n", portno);
    }

    //clients
    int clients[MAX_CLIENTS];
    fd_set fds;

    //initialization of clients
    int i = 0;
    for (i = 0; i < MAX_CLIENTS; i++){
        clients[i] = 0;
    }

    //start listening
    if (listen(sockfd, 10) < 0){
        printf("ERROR: Listen failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket listening started\n");
    }

    //enter main loop
    int addrlen = sizeof(serv_addr);
    printf("Server Address Length: %d\n", addrlen);

    int maxsd = 0;
    int activity = 0;
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
            int new_socket, cli_addrlen;
            cli_addrlen= sizeof(cli_addr);
 //           ;
            if ((new_socket = accept(sockfd, (struct sockaddr *)&cli_addr,
                    (socklen_t*)&cli_addrlen)) < 0){
                printf("ERROR: Accept connection failed\n");
            }else{
                printf("Abc\n");
                printf("Connection established: socket %d, ip %s, port %d \n",
                        new_socket, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
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
        portno = 12345;
    }else{
        portno = strtol(argv[1],NULL,10);
        if (errno!=0){
            fprintf(stderr,"WARNING: Port provided is invalid. Using 12345 as default\n");
            portno = 12345;
        }
    }
    printf("Using port %d\n",portno);
    return portno;
}


struct sockaddr_in getservaddr(int portno){
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    return serv_addr;
}
