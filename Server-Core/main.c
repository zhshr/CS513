#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "main.h"


int main(int argc, char* argv[]){
    int sockfd, newsockfd, portno, clilen, n;
    char buffer[256];

    struct param p;
    pthread_t pth;
    //Determines the port number
    portno = getportno(argc, argv);
    p.portno = portno;
    pthread_create(&pth, NULL, core_thread_main, (void*)&p);
    sleep(10);
    return 0;
}

void *core_thread_main(void *pParam){
    struct param p;
    p = *(struct param*)pParam;
    int sockfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    printf("Thread Started\n");

    //set port number and bind port
    portno = p.portno;
    sockfd = getsockfd();
    serv_addr = getservaddr(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("ERROR: Port binding failed\n");
        exit(0);
    }else{
        printf("Port binding successfully\n");
    }

    //start listening
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

int getsockfd(){
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        printf("ERROR: Failed creating socket.");
        exit(0);
    }
    printf("Socket File Descripter: %d\n", sockfd);
    return sockfd;
}

struct sockaddr_in getservaddr(int portno){
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    return serv_addr;
}
