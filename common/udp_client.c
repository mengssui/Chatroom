/*************************************************************************
	> File Name: udp_client.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 06:18:55 PM CST
 ************************************************************************/

#include "head.h"
extern int port;

int socket_udp() {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }

    return sockfd;
}

int udp_connect(struct sockaddr_in *client) {
    int sockfd;
    if ((sockfd = socket_create_udp(port)) < 0) { //根据固定的port创建fd
        return -1;
    }

    //客户端发现线程创建时候是随机端口，因此如果发回去，没有监听对应端口的程序，只有监听8888端口
    client->sin_port = htons(8888); //改为8888

    if (connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) <0) {
        return -1;
    }

    return sockfd;
}


