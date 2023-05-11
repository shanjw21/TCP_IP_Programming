#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc,char *argv[]){
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handling("socket() error ");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }
    
    if(listen(serv_sock,5) == -1)
        error_handling("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);
    // 为处理5个客户端连接而添加的循环语句，调用5次accept函数，依次向5个客户端提供服务。
    for(i = 0; i < 5; i++){
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error");
        else    printf("Connected client %d \n",i+1);

        // 完成回声服务，原封不动的传输读取的字符串
        while((str_len = read(clnt_sock,message,BUF_SIZE))!= 0)
            write(clnt_sock,message,str_len);

        // 针对套接字调用close函数，向连接的相应套接字发送EOF。
        close(clnt_sock);
    }
    // 完成5个客户端提供服务后关闭服务器端套接字并终止程序
    close(serv_sock);

    return 0;
}

void error_handling(char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}