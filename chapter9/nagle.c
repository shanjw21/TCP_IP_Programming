#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/tcp.h>
#define TRUE 1
#define FALSE 0

/*
    the value of TCP_NODELAY is 0 Nagle 默认是0
    the value of TCP_NODELAY is 1 禁用Nagle算法设置TCP_DELAY可选项为1.
*/

void error_handing(char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc, char** argv){
    int optval;
    socklen_t optlen;

    int sock;
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error_handing("socket() error.");

    int state;
    optlen = sizeof(optval);
    state = getsockopt(sock, IPPROTO_TCP,TCP_NODELAY,(void*)&optval,&optlen);
    if(state)
        error_handing("getsockopt() error.");
    printf("the value of TCP_NODELAY is %d \n", optval);
    
    optval = TRUE;
    state = setsockopt(sock,IPPROTO_TCP,TCP_NODELAY,(void*)&optval, optlen);
    if(state)
        error_handing("getsockopt() error.");
    
    optlen = sizeof(optval);
    state = getsockopt(sock, IPPROTO_TCP,TCP_NODELAY,(void*)&optval,&optlen);
    if(state)
        error_handing("getsockopt() error.");
    printf("the value of TCP_NODELAY is %d \n", optval);
    return 0;
}