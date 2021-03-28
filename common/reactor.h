/*************************************************************************
	> File Name: reactor.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 08:14:51 PM CST
 ************************************************************************/

#ifndef _REACTOR_H
#define _REACTOR_H
int add_to_sub_reactor(int fd, struct User *user);
void *reactor(void *arg);
void del_from_reactor(int fd);
#endif
