/*************************************************************************
	> File Name: udp_client.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 06:19:02 PM CST
 ************************************************************************/

#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

int socket_udp();
int udp_connect(struct sockaddr_in *client);
#endif
