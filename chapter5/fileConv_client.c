#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ip_address> <port> <file_name>\n", argv[0]);
        exit(1);
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);
    char *file_name = argv[3];

    // 创建socket
    int client_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(1);
    }

    // 连接服务器
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address);
    server_addr.sin_port = htons(port);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("connect");
        exit(1);
    }

    // 发送文件名
    send(client_fd, file_name, strlen(file_name), 0);


    char filename[100] = "new";
    strcat(filename, file_name);
    // 打开文件
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        perror("fopen");
        exit(1);
    }

    char buffer[MAX_SIZE];
    int len;

    while ((len = recv(client_fd,buffer,MAX_SIZE,0)) > 0){
        fwrite(buffer,sizeof(char),len,fp);
    }

    // 关闭文件和socket
    fclose(fp);
    close(client_fd);

    printf("File recived successfully.\n");

    return 0;
}
