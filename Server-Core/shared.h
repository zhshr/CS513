#ifndef HEADER_SHARED
#define HEADER_SHARED

#define MAX_CLIENTS 100
#define BUFFER_SIZE 10240
#define TRUE 1
#define FALSE 0

struct param{
    int portno;
};
int getportno(int argc, char* argv[]);
struct sockaddr_in getservaddr(int portno);
void *core_thread_main(void *pParam);
int setupmastersocket(int portno);
#endif

