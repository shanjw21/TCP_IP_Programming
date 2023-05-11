#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
void error_handing(char* message);

/*
    setsockopt函数更改可选项值。
    输出：
    Input buffer : 6144 
    output buffer : 16384
    缓冲大小的设置不会完全按照外面设定执行，setsockopt函数向系统传递我们的要求。
*/

int main(int argc, char** argv){
    int sock;
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;
    
    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error_handing("socket() error");
    
    int state;
    // setsockopt函数将rcv_buf中的值保存到可选项中。更改原始可选项值
    state = setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&rcv_buf,sizeof(rcv_buf));
    if(state)
        error_handing("setsockopt() error");
    
    socklen_t len;
    len = sizeof(snd_buf);
    state = getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&snd_buf,&len);
    if(state)
        error_handing("setsockopt() error");

    state = getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&rcv_buf,&len);
    if(state)
        error_handing("setsockopt() error");
    
    printf("Input buffer : %d \n",rcv_buf);
    printf("output buffer : %d \n",snd_buf);
    return 0;
}


void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}