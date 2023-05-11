#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handing(char *message);

int main(int argc,char*argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if(argc!= 3){
        printf("Usage : %s <IP> <port>\n",argv[0]);
        exit(1);
    }

    // 创建 IPv4协议族中面向连接的套接字, 创建TCP套接字，前两个参数是PF_INET、SOCK_STREAM时，可以省略IPPROTO_TCP
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sock == -1)
        error_handing("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // connect函数向服务器发送连接请求
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
        error_handing("connect() error");

    // read函数读取数据，成功时返回接收的字节数，失败返回-1
    // 反复调用read函数，每次读取1字节，read返回0，说明遇到了文件结尾，
    while(read_len=read(sock,&message[idx++],1)){
        if(read_len == -1)
            error_handing("read() error");
        // 累加read调用接收到的信息的字节数。
        str_len += read_len;
    }

    printf("Message from server : %s \n", message);
    // 由于一次读取一个字节，所以传递了多少数据说明调用了多少此read
    printf("Function read call count : %d \n",str_len);
    close(sock);
    return 0;
}

void error_handing(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}