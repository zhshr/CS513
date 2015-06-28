#include "shared.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>

struct sockaddr_in getservaddr(int portno);

int createsocket(int portno, int range, int server){
    int sockfd;
    char temp[100];
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
    int port = portno - 1;
    do{
        port++;
        if (port>portno + range){
            mylog("No port available");
            exit(EXIT_FAILURE);
        }
        sprintf(temp, "Trying to bind port %d\n", port);
        mylog(temp);
        sock_addr = getservaddr(port);
    }while (bind(sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0);
    printf("Port binding successfully, port %d\n", port);
    if (server==TRUE){
        setsockforserver(sockfd);
        int addrlen = sizeof(sock_addr);
        printf("Server Address Length: %d\n", addrlen);
    }else{
        setsockforclient(sockfd);

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

int setsockforclient(int sockfd){

}
struct sockaddr_in getservaddr(int portno){
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    return serv_addr;
}

int socksockforclient(int sockfd){
}
