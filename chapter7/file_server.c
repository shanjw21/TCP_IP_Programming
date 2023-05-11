#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char* message);

int main(int argc, char* argv[]){
    FILE * fp;
    int serv_sd,clnt_sd;
    struct sockaddr_in serv_adr,clnt_adr;
    socklen_t clnt_adr_sz;
    char buf[BUF_SIZE];
    int read_cnt;

    if(argc != 2){
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }
    // 二进制文件读, 打开文件以向客户端传输服务器端源文件file_server.c
    fp = fopen("file_server.c","rb");
    serv_sd = socket(PF_INET,SOCK_STREAM,0);

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sd,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    if(listen(serv_sd,5) == -1)
        error_handling("listen() error");
    
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sd = accept(serv_sd,(struct sockaddr*)&clnt_adr,&clnt_adr_sz);

    if(clnt_sd == -1)
        error_handling("accept() error");
    else
        printf("Connected......");
    // 向客户端传输文件数据的循环语句。客户端是accept函数调用中连接的
    while(1){
        read_cnt = fread((void*)buf, 1, BUF_SIZE,fp);
        if(read_cnt < BUF_SIZE){
            write(clnt_sd, buf,read_cnt);
            break;
        }
        write(clnt_sd,buf,BUF_SIZE);
    }
    // 发送完文件后针对输出流进行半关闭
    shutdown(clnt_sd,SHUT_WR);
    // 只关闭了输出流，还可以通过输入流接收数据
    read(clnt_sd,buf,BUF_SIZE);
    printf("Message from client: %s \n",buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}

void error_handling(const char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}