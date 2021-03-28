/*************************************************************************
	> File Name: tcp_client.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 01 Dec 2020 06:23:52 PM CST
 ************************************************************************/

#include "head.h"

int socket_connect(char* ip, int port) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        //perror("socket");
        return -1;   //因为这是一个函数，更希望调用这个函数的上层得到错误提示，而不是在这个位置直接perror
    }
     
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        return -1;
    }
    return sockfd;

}


int socket_connect_timeout(char* ip, int port, long time_out) {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        //perror("socket");
        return -1;   //因为这是一个函数，更希望调用这个函数的上层得到错误提示，而不是在这个位置直接perror
    }
     
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    make_nonblock(sockfd);  //非阻塞
     

    //设置select
    fd_set wfds;   //客户端连服务器，去看能不能写
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = time_out;

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        int error;
        socklen_t len = sizeof(error);
        int ret = select(sockfd + 1, NULL, &wfds, NULL, &timeout);
        if (ret < 0) {
            close(sockfd);
            return -1;
        } else if (ret) {
            if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {  //获取socket是否错误，错误显示在error中，来判断网络不稳定的时候可能出现的问题
                close(sockfd);
                return -1;
            }
            if (error) {
                close(sockfd);
                return -1;
            }
            printf("connected!\n");
        } else {
            printf("TimeOut!\n");
            close(sockfd);
            return -1;
        }
    }
    
    make_block(sockfd);

    return sockfd;
}

