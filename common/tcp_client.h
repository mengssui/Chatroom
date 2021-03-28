/*************************************************************************
	> File Name: tcp_client.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 01 Dec 2020 06:21:26 PM CST
 ************************************************************************/

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H
/*
* 连接
*/
int socket_connect(char* ip, int port);
int socket_connect_timeout(char* ip, int port, long usec);

#endif
