#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
    int getsockopt(int sock,int level, int optname, void *optval, socklen_t* optlen);
    功能：获取套接字sock的可选项optname的值，并保存到optval中查看。
*/

void error_handing(char*message);

int main(int argc, char** argv){

    int tcp_sock = socket(PF_INET,SOCK_STREAM,0);
    int udp_sock = socket(PF_INET, SOCK_DGRAM,0);
    printf("SOCK_STREAM : %d \n",SOCK_STREAM);
    printf("SOCK_DGRAM : %d \n",SOCK_DGRAM);

    int sock_type;
    socklen_t optlen;
    optlen = sizeof(sock_type);
    int state = getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(state){
        error_handing("getsockopt() error.");
    }
    printf("Socket type one : %d \n",sock_type);

    state = getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(state){
        error_handing("getsockopt() error.");
    }
    printf("Socket type one : %d \n",sock_type);
    return 0;
}

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}