#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define BUF_SIZE 1024
void error_handing(char* message);

/*
    binding error 的原因与解决方案:
    原因：  为防止TCP传输过程中，主动断开的主机最后传递的ack在传输过程中丢失，所以会存在time-wait状态，
    在此状态下，套接字还处于被占用中，不能被使用，此时重新赋予主机这个套接字会出现bind error
    解决方案：  为了避免因为服务器停机造成的tie-wait时间过长，可以认为设置time-wait时间，通过更改SO_REUSEADDR的状态来给处于time-wait状态下的
    套接字端口号重新分配给新的套接字。SO_REUSEADDR默认值为0，可以将其改为1.从而可以略过time-wait时间直接将原端口赋予新的套接字。
*/

int main(int argc, char** argv){
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t len,clnt_sz;
    int str_len;
    char message[BUF_SIZE];

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[1]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handing("socket() error.");

    int option,state;
    socklen_t optlen;
    optlen = sizeof(option);
    state = setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,optlen);
    if(state)
        error_handing("setsockopt() error.");

    memset(&serv_adr,0,sizeof serv_adr);
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)))
        error_handing("bind() error.");

    if(listen(serv_sock,10) == -1)
        error_handing("listen() error.");
    
    clnt_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_sz);
    while((str_len = read(clnt_sock,message,sizeof(message)))!=0){
        write(clnt_sock,message,str_len);
        write(1,message,str_len);
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handing(char* message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}