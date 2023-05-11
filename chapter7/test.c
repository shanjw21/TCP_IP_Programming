#include <stdio.h>
#include <string.h>

#define BUF_SIZE 30
// 测试c标准库函数fread
// size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream);
void test1(){
    FILE* fp;
    char c[] = "This is runoob.";
    char buffer[BUF_SIZE];

    // 打开文件用于读写，w+表示打开文件用于读写没文件存在就截去
    fp = fopen("file.txt","w+");

    // 写入数据到文件
    size_t len = fwrite(c,strlen(c) + 1, 1,fp);
    printf("%ld\n",len);

    printf("%ld\n",strlen(c) + 1);

    // 查找文件的开头
    fseek(fp,0,SEEK_SET);

    // 读取并显示数据
    fread(buffer,strlen(c) + 1, 1, fp);
    printf("%s\n",buffer);
    fclose(fp);

}

int main(int argc, char* argv[]){
    test1();

    return 0;
}