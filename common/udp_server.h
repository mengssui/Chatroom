/*************************************************************************
	> File Name: udp_server.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 07:16:17 PM CST
 ************************************************************************/

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

int socket_create_udp(int port);
int udp_accept(int fd, struct User *user);

#endif
