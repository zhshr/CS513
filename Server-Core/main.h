#ifndef HEADER_MAIN
#define HEADER_MAIN 1

struct param{
    int portno;
};
int getportno(int argc, char* argv[]);
int getsockfd();
struct sockaddr_in getservaddr(int portno);
void *core_thread_main(void *pParam);
#endif

