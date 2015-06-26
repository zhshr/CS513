#include "shared.h";

char *processmessage(char *buffer, int index, int *clients[], char (*nicknames[])[]){
    char *s;
    s = malloc(BUFFER_SIZE+1);
    bzero((char *)s, sizeof(s));
    printf("Start process message\n");
    sprintf(s, "%s: You typed %s", *nicknames[index], buffer);
    if (sendtosocket(clients[index], s) < 0){
        printf("ERROR: Send message to client no.%d, fd %d failed\n", index, clients[index]);
    }
    free(s);
}
int sendtosocket(int sockfd, char *message[]){
    return (send(sockfd, message, strlen(message),0)!=strlen(message));
}
