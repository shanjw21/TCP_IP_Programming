/*
    分割TCP的I/O程序。分割数据的收发过程。
    客户端父进程负责接收数据，创建的子进程负责发送数据，分割后，不同进程分别负责输入和输出。
    分割I/O程序的好处在于，可以提高频繁交换数据的程序性能。客户端不用考虑接收数据的情况，有持续发送数据，可以提高同一时间内
    传输的数据量。
*/
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define BUF_SIZE 1024

void error_handing(char* message);
void write_routine(int sock,char* buf);
void read_routine(int sock,char* buf);

int main(int argc, char** argv){

    int serv_sock;
    struct sockaddr_in serv_adr;
    pid_t pid;
    char buf[BUF_SIZE];
    
    // 创建服务端套接字
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handing("socket() error.");

    // 初始化服务端套接字地址，协议，IP，port
    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

    // 向服务端发送连接请求。
    if(connect(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
        error_handing("connect() error!");
    else    
        puts("Connected......");
    
    pid = fork();
    if(pid == 0){ // 子进程
        write_routine(serv_sock,buf);
    }
    else    
        read_routine(serv_sock,buf);
    
    close(serv_sock);
    return 0;
}

// 数据输入I写入
void write_routine(int sock, char* buf){
    while(1){
        fgets(buf,BUF_SIZE,stdin);
        if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n")){
            shutdown(sock,SHUT_WR); // 由于父进程复制了一份描述符，所以需要两次关闭套接字描述符。
            return;
        }
        write(sock,buf,strlen(buf));
    }
}

// 数据输出O读取
void read_routine(int sock,char* buf){
    while(1){
        int str_len = read(sock,buf,BUF_SIZE);
        if(str_len == 0){
            return;
        }
        buf[str_len] = '\0';
        printf("Message from server : %s\n",buf);
    }
}

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}