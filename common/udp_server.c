/*************************************************************************
	> File Name: udp_server.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 06:18:38 PM CST
 ************************************************************************/
#include "head.h"
extern int subfd;
extern char name[20];

int socket_create_udp(int port) {
    int listener;
    if ((listener = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        return -1;
    }
    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    unsigned long opt = 1;
    //设置端口重用
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listener, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }
    return listener;
}

int udp_accept(int fd, struct User *user) {
    int new_fd, ret;
    struct sockaddr_in client;
    struct Msg request, response;
    socklen_t len = sizeof(client);

    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));

    //收数据并且记录客户端的信息,端口、ip等
    ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);

    if (ret != sizeof(request)) {  //请求包的大小和期望大小不同
        DBG(RED"Package size Error\n"NONE);
        response.type = CHAT_FIN; //关闭请求
        strcpy(response.msg, "Login Requst Error!"); 
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;  //上层需要关闭fd
    }

    //检查对方是否在线, 重复登陆的问题
    /*if (check_online(&request)) {
        response.type = CHAT_FIN;
        strcpy(response.msg, "Already login!");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }*/

    if (request.type & CHAT_FIN) {
        DBG(RED"request type CHAT_FIN Error\n"NONE);
        return -1;
    } else if (request.type & CHAT_SYN && request.type & CHAT_ACK) { //作为客户端角色发送完SYN后接收到对方的SYNACK
        DBG(GREEN"<Accepter>"NONE" : SYNACK recived!\n");
    } else if (request.type != CHAT_SYN) {   //作为服务器角色接收到对方的SYN请求包
        DBG(RED"request type SYN Error<0X%x\n"NONE, request.type);
        response.type = CHAT_FIN;
        strcpy(response.msg, "Error Request!");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }

    DBG(PINK"[msg] : %s\n"NONE, request.msg);

    //新创建一个sockfd连接对方的指定的端口,这样以后不通过主反应堆接收这个8888端口的套接字
    new_fd = udp_connect(&client);
    if (new_fd < 0) {
        perror("udp_connect");
        return -1;
    }

    strcpy(user->name, request.from);
    strcpy(user->ip, inet_ntoa(client.sin_addr));
    user->flag = FL_ONLINE;
    user->cnt = 5;
    user->fd = new_fd;

    char buff[512]  = {0};
    sprintf(buff, "%s Login success!\n", user->name);
    strcpy(response.msg, buff);
    response.type = CHAT_ACK | CHAT_SYN;
    strcpy(response.from, name);
    send(new_fd, (void *)&response, sizeof(response), 0);
    return new_fd;
}
