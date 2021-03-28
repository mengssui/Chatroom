/*************************************************************************
	> File Name: ./common/send_chat.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Mon 18 Jan 2021 07:53:54 PM CST
 ************************************************************************/
#include "head.h"
extern char name[20];
extern struct User *users;

void *send_chat(void *arg) {
    struct Msg msg;
    strcpy(msg.from, name);
    msg.type = CHAT_MSG;
    while (1) {
        memset(msg.msg, 0, sizeof(msg.msg));
        printf("Please input msg :\n");
        scanf("%[^\n]", msg.msg);
        getchar();  //吞掉别的
        for (int i = 0; i < MAXUSER; i++) {
            if (users[i].flag) {
                send(users[i].fd, &msg, sizeof(msg), 0);
            }
        }
    }
}

