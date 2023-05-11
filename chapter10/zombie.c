#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*
    进程完成后如果不销毁则会继续占用系统资源变成僵尸进程。
    子进程终止方式：
        传递参数并调用exit函数。
        main函数中执行return语句并返回值。
    上述两种返回值都会传递给操作系统，操作系统不会销毁子进程而是会将返回值传递给产生子进程的父进程。此状态下的子进程就是僵尸进程。
    僵尸进程由操作系统产生，其销毁的时机是：向创建子进程的父进程传递子进程的exit参数值或return语句的返回值。只有父进程主动请求时
    操作系统参会将这些返回值传递给父进程。
    父进程终止，处于僵尸状态的子进程将同时销毁。
*/

int main(int argc, char** argv){
    pid_t pid = fork();
    if(pid == 0){
        puts("Hi, I am a child process.");
    }
    else{
        printf("Child process ID : %d \n",pid);
        sleep(30);
    }

    if(pid == 0)
        puts("end child process.");
    else    
        puts("end parent process.");
    return 0;
}