#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
void error_handing(char* message);

/*
    输出：
    Input buffer : 131072 
    output buffer : 16384
*/

int main(int argc, char**argv){
    int sock;
    socklen_t len;

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error_handing("socket()");
    int snd_buf, rev_buf,state;
    len = sizeof(snd_buf);
    // 获取SOL_SOCKET协议层的SO_SNDBUF可选项属性存储到snd_buf中
    state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&snd_buf,&len);
    if(state)
        error_handing("getsockopt()");
    
    len = sizeof(rev_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&rev_buf,&len);
    if(state)
        error_handing("getsockopt()");
    
    printf("Input buffer : %d \n",rev_buf);
    printf("output buffer : %d \n",snd_buf);
    return 0;
}

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}