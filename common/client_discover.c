/*************************************************************************
	> File Name: common/client_discover.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Sat 09 Jan 2021 06:48:45 PM CST
 ************************************************************************/
#include "head.h"

extern char* config;
extern char name[20];
extern char msg[512];

//线程函数,给所有的主机发送SYN
void *client_discover(void* arg) {
    char file[512] = {0};
    FILE* fp = NULL;
    struct Msg request;
    struct sockaddr_in client;
    int sockfd;

    if ((sockfd = socket_udp()) < 0) { //创建udp套接字,端口是随机的
        perror("socket_udp");
        exit(1);
    }

    //根据配置文件找到iplist的路径
    strcpy(file, get_value(config, "iplist"));

    if ((fp = fopen(file, "r")) == NULL) { //是否打开文件成功
        perror("fopen");
        exit(1); //整个程序退出
    }

    //填写请求的包
    strcpy(request.from, name);
    strcpy(request.msg, msg);
    request.type = CHAT_SYN;
    
    client.sin_family = AF_INET;
    client.sin_port = htons(8888);

    DBG("<Ip Discover> start...\n");
    while(!feof(fp)) {
        char ip[20] = {0};
        fscanf(fp, "%s", ip);
        if (!strlen(ip)) continue; //会出现最后读取空的现象，以此剔除
        client.sin_addr.s_addr = inet_addr(ip);
        //发送SYN发现包,使用udp从本机的sockfd套接字发送到client指定的地址及端口
        sendto(sockfd, (void*)&request, sizeof(request), 0, (struct sockaddr*)&client, sizeof(client));
    }
    DBG("<Ip Discover> end...\n");
}
