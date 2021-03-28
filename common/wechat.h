/*************************************************************************
	> File Name: wechat.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 07:48:50 PM CST
 ************************************************************************/

#ifndef _WECHAT_H
#define _WECHAT_H
#define MAXUSER 1000

//位掩码 -> 必须是只有一个是1的
#define CHAT_SYN   0x001
#define CHAT_ACK   0x002
#define CHAT_FIN   0x004
#define CHAT_HEART 0x008
#define CHAT_MSG   0x010
#define CHAT_PRI   0x020

struct Msg {
    int type;
    char from[20];
    char to[20];
    char msg[1024];
};

#define FL_SYNSENT 0x001
#define FL_ONLINE  0x002
struct User {
    char name[20];
    char ip[20];
    int flag;       //是否在线，用户状态
    int fd;
    int cnt;        //发出心跳的次数, 达到10时候删除
};

#endif
