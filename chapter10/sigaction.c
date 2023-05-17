/*
    信号处理函数：对信号做出反应。
    #include <signal.h>
    int sigaction(int signo, const struct sigaction* act, struct sigacton* oldact);
        -功能：成功返回0，失败返回-1.
        -参数：
            -signo：信号信息
            -act：信号处理函数
            -oldact:获取注册的信号处理函数指针，不需要传递0
    sigaction结构体：
    struct sigaction{
        void(*sa_handler)(int); 保存
        sigset_t sa_mask;
        int sa_flags;
    }
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#define _XOPEN_SOURCE 700

// 信号处理函数，当发生sig信号时，调用这个函数。
void timeout(int sig){
    if(sig == SIGALRM){
        puts("Time out!");
    }
    alarm(2);
}

int main(){

    // sigaction 结构体
    struct sigaction act;
    act.sa_handler = timeout; // 注册信号处理函数。
    sigemptyset(&act.sa_mask); // sa_mask用来指定信号相关的选项。
    act.sa_flags = 0; // 指定信号相关的特性。

    sigaction(SIGALRM,&act,0);
    alarm(2);

    for(int i = 0; i < 3; i++){
        puts("wait...");
        sleep(100);
    }
    return 0;
}