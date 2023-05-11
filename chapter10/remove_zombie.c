/*
    使用sigaction信号处理函数处理僵尸进程。
    子进程终结时产生SIGCHLD信号。
    使用信号处理函数处理子进程退出后变为僵尸进程的问题。
*/
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

void error_handing(char* message);

void read_childproc(int sig){
    int status;
    pid_t id = waitpid(-1,&status,WNOHANG);
    if(WIFEXITED(status)){
        printf("removed proc id: %d\n",id);
        printf("returned status is : %d\n",WEXITSTATUS(status));
    }
}

int main(int argc, char** argv){

    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD,&act,0);

    pid_t pid = fork();
    if(pid == -1){
        error_handing("fork() error in first child process.\n");
    }
    else if(pid == 0){
        printf("Hi, I am child process! pid : %d\n",getpid());
        sleep(10);
        return 12;
    }
    else{
        printf("child proceess1 pid : %d\n",pid);
        pid = fork();
        if(pid == -1)
            error_handing("fork() error in second child process.\n");
        else if(pid == 0){
            printf("Hi, I am child process! pid : %d\n",getpid());
            sleep(10);
            exit(24);
        }
        else{
            printf("child process2 pid : %d\n",pid);
            for(int i = 0; i < 5; i++){
                puts("wait...\n");
                sleep(5);
            }
        }
    }
    return 0;
}

void error_handing(char* message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
