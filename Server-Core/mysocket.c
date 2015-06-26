#include "shared.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
int createsocket(int portno, int server){
    int sockfd;
    printf("create socket\n");
    struct sockaddr_in sock_addr;

    //set port number and bind port
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd<0){
        printf("ERROR: Failed creating socket.");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket File Descripter: %d\n", sockfd);
    }

    //bind port
    sock_addr = getservaddr(portno);
    if (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0){
        printf("ERROR: Port binding failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Port binding successfully, listening on port %d\n", portno);
    }

    if (server==TRUE){
        setsockforserver(sockfd);
        int addrlen = sizeof(sock_addr);
        printf("Server Address Length: %d\n", addrlen);
    }else{

    }
    return sockfd;
}

int setsockforserver(int sockfd){
    int opt = TRUE;
    //set socket to accept multiple connections
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0){
        printf("ERROR: Set Reuse Addr failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Set Reuse Addr successful\n");
    }

    //start listening
    if (listen(sockfd, 10) < 0){
        printf("ERROR: Listen failed\n");
        exit(EXIT_FAILURE);
    }else{
        printf("Socket listening started\n");
    }
}

int socksockforclient(int sockfd){
}
