/*************************************************************************
	> File Name: tcp_server.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 01 Dec 2020 07:06:21 PM CST
 ************************************************************************/

#include "head.h"

int socket_create(int port) {
    int server_listen;
    if ((server_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");  //任何IP都接收

    if (bind(server_listen, (struct sockaddr*)&server, sizeof(server)) < 0) {
        return -1;
    }

    if (listen(server_listen, 20) < 0) {
        return -1; 
    }
    return server_listen;
}
