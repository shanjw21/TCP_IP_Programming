#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*
    进程：占用内存空间正在运行的重新，进程是程序六的基本单位。
    CPU中运算器个数是核的个数。
    #include <unistd.h>
    #include <sys/types.h> 
    pid_t fork(void); // 创建正在运行的程序副本，新创建的进程执行fork函数调用后的语句，之后的程序流根据fork函数的返回值加以区分.
    父进程：fork函数返回子进程ID,
    子进程：fork函数返回0

    输出：Child Proc: [9,23] 
         Child Proc: [13,27] 
        fork函数调用后分成了两个不同的进程，父子进程拥有完全独立的内存结构，二者只是共享同一代码。
*/

int gval = 10; // 全局变量
int main(int argc, char** argv){
    pid_t pid;
    int lval = 20;
    gval++, lval+=5;

    pid = fork(); // 创建子进程
    if(pid == 0){
        gval += 2, lval += 2; // 子进程执行
    }
    else    
        gval -= 2, lval -= 2; // 父进程执行

    if(pid == 0)
        printf("Child Proc: [%d,%d] \n",gval, lval);
    else    
        printf("Child Proc: [%d,%d] \n",gval, lval);
    return 0;
}