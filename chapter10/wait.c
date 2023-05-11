#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

/*
    通过wait函数销毁僵尸进程：
    #icnlude <sys/wait.h>
    pid_t wait(int* statloc);
*/

int main(int argc, char** argv){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        return 3; // 子进程通过return终止并向操作系统传递返回值 3。
    }else{
        printf("Child PID : %d \n",pid);
        pid = fork();
        if(pid == 0){
            exit(7); // 通过exit终止并向os传递返回值7
        }
        else{
            printf("Child Process pid : %d \n",pid);
            wait(&status); // 父进程调用wait等待子进程退出，并向os申请得到子进程终止返回值,并将信息保存到status中
            if(WIFEXITED(status)){ // 调用宏函数WIFEXITED检查子进程是否正常终止，正常终止返回true
                printf("Child send one : %d \n",WEXITSTATUS(status)); // 调用宏函数WEXITSTATUS返回status中保存的子进程终止时传递的返回值。
            }

            // 创建了2个子进程，所以再次调用wait函数和宏
            printf("Child Process pid : %d \n",pid);
            wait(&status); // 父进程调用wait等待子进程退出，并向os申请得到子进程终止返回值,并将信息保存到status中
            if(WIFEXITED(status)){ // 调用宏函数WIFEXITED检查子进程是否正常终止，正常终止返回true
                printf("Child send two : %d \n",WEXITSTATUS(status)); // 调用宏函数WEXITSTATUS返回status中保存的子进程终止时传递的返回值。
            }
            sleep(30);// 暂停父进程终止，以便查看子进程状态。
        }
    }
    return 0;
}