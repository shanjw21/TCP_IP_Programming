/*
    多进程实现回声服务器，主线程负责连接客户端，子线程负责用于同客户端通信。
    父进程向子进程传递两个套接字文件描述符，套接字是属于操作系统的。所以子进程需要关闭服务端套接字描述符。
    套接字并非进程所有。
*/

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <wait.h>
#include <signal.h>
#define BUF_SIZE 1024

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

// 信号处理函数，用于父进程回收子进程资源。避免子进程变成僵尸进程。
void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1,&status,WNOHANG);
    if(WIFEXITED(status)){
        printf("removed child process pid : %d\n",pid);
        printf("child send : %d\n",WEXITSTATUS(status));
    }
}

int main(int argc, char** argv){

    if(argc != 2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr,clnt_adr;
    socklen_t clnt_len;
    pid_t pid;
    int str_len;
    char buf[BUF_SIZE];
    int status;
    // 注册信号处理函数。
    struct sigaction act;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    status = sigaction(SIGCHLD,&act,0); // 成功返回0，失败返回-1
    if(status == -1)
        error_handing("sigaction() error\n");
    // 初始化服务端套接字地址
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    // 创建套接字
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
        error_handing("socket() error.");
    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
        error_handing("bind() error.");
    if(listen(serv_sock,10) == -1)
        error_handing("listen() error.");

    while(1){
        clnt_len = sizeof(clnt_adr);
        // 负责与客户端通信的套接字。
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_adr,&clnt_len);
        if(clnt_sock == -1)
            continue; // 创建失败，跳过。
        else    
            puts("a new client connected...");
        pid = fork();
        if(pid == -1){ // 创建失败关闭通信套接字，跳过。
            close(clnt_sock);
            continue;
        }
        // 子进程负责与连接到的客户端通信
        else if(pid == 0){
            close(serv_sock); // 子进程中关闭负责连接的服务端套接字。
            while((str_len = read(clnt_sock,buf,BUF_SIZE)) != 0){ // 错误点。
                write(clnt_sock,buf,str_len);
            }

            printf("client[%s:%d] disconnected, conn_id=%d\n", inet_ntoa(clnt_adr.sin_addr), 
                    ntohs(clnt_adr.sin_port), clnt_sock);
            
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        else{
            printf("New child proc ID: %d\n", pid);
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}