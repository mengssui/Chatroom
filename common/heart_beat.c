/*************************************************************************
	> File Name: heart_beat.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Mon Jan 18 18:50:41 2021
 ************************************************************************/
#include "head.h"
extern struct User *users;
extern char name[20];

void *heart_beat(void* arg) {
    struct Msg heart;
    heart.type = CHAT_HEART;
    strcpy(heart.msg, "mengbw's heart beat!");
    while(1) {
        for (int i = 0; i < MAXUSER; i++) {
            if (users[i].flag) {  //用户在线
                if (users[i].cnt <= 0) {
                    DBG("<%s>"NONE" is removed by heart beat...\n",users[i].name);
                    users[i].flag = 0;
                    del_from_reactor(users[i].fd);
                    DBG("<%s>"NONE" is removed by sub reactor...\n",users[i].name);
                    close(users[i].fd);
                }
                strcpy(heart.from, name);
                send(users[i].fd, (void*)&heart, sizeof(heart), 0);
                users[i].cnt--;
            }
        } 
        sleep(10); //两秒钟发一次
    }
}
