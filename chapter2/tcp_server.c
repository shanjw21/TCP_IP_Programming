#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
void error_handling(char*message);

int main(int argc,char *argv[]){

    int serv_sock;
    int clnt_sock;

    // 定义在头文件 <arpa/inet.h>中,定义了Inernet socket address
    // sockaddr_in表示IPv4地址的结构体
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    char message[] = "Hello World!";

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    // PF_INET ，IP协议簇，#define PF_INET 2, PF_INET表示IPv4
    // SOCK_STREAM 有序的、可信赖的、基于连接的字节流，TCP连接
    // 调用socket函数生成 服务器socket套接字
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    // socket函数成功返回文件描述符，失败返回-1
    if(serv_sock == -1)
        error_handling("socket() error");
    
    // sin_family 端口号，#define AF_INET PF_INET
    memset(&serv_addr,0,sizeof(serv_addr));
    // 地址族选择IPv4地址族
    serv_addr.sin_family = AF_INET;
    // 32位ip地址
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 端口号
    serv_addr.sin_port = htons(atoi(argv[1]));

    // bind函数将创建好的套接字分配IP地址和端口号
    if(bind(serv_sock,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) == -1)
        error_handling("listen() error");
    // listen函数将套接字转化为可接受连接状态
    if(listen(serv_sock,5) == -1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);
    // accept函数受理连接请求，如果在没有连接请求的情况下调用该函数，则不会返回，直到有连接请求为止
    clnt_sock= accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    //稍后要将介绍的 write 函数用于传输数据，若程序经过 accept 这一行执行到本行，则说明已经有了连接请求
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    // 将字符串写入 stderr 流中
    fputs(message, stderr);
    // 将字符写入erroe stream中
    fputc('\n', stderr);
    exit(1);
}