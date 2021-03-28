/*************************************************************************
	> File Name: reactor.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 08:12:19 PM CST
 ************************************************************************/
#include "head.h"
extern struct User *users;
extern char name[20];
extern int subfd;

void add_event_ptr(int epollfd, int fd, int events, struct User *user) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = (void *)user;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    printf(BLUE"add one user into subfd [name]%s -> ip[%s]\n"NONE, user->name, user->ip);
}

int add_to_sub_reactor(int fd, struct User *user) {
    int sub = user->fd;
    users[sub] = *user;
    add_event_ptr(fd, users[sub].fd, EPOLLIN | EPOLLET, &users[sub]);
    return -1;
}

void del_from_reactor(int fd) {
    epoll_ctl(subfd, EPOLL_CTL_DEL, fd, NULL);
}

//从反应堆中
void *reactor(void *arg) {
    srand(time(0));
    struct epoll_event events[20]; //内核会修改，可以不清空
    struct Msg msg, respose;
    strcpy(respose.from, name);
    while (1) {
        int nfds = epoll_wait(subfd, events, 20, -1);  
        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < nfds; i++) {
            int fd = ((struct User*)(events[i].data.ptr))->fd;
            recv(fd, &msg, sizeof(msg), 0);
            users[fd].cnt = 5;
            if (msg.type & CHAT_HEART) { //心跳包
                DBG(BLUE"<%s>"NONE"💜HEART💜 -> "BLUE"<Me>"NONE" \n",users[fd].name);
                msg.type = CHAT_ACK;
                send(fd, &msg, sizeof(msg), 0);
            } else if (msg.type & CHAT_ACK) { //应答包
                DBG(BLUE"<%s>"NONE "👩ACK👩 -> <ME>\n",users[fd].name);
            } else if(msg.type & CHAT_FIN) {   //挥手
                //epoll_ctl delete from sub reactor
                del_from_reactor(fd);
                close(fd);
                users[fd].flag = 0;
                DBG(RED"FIN <%s> is removed from sub reactor\n"NONE, users[fd].name);
            } else if (msg.type & CHAT_MSG ) {

                DBG(PINK"【Message】"NONE" <%s> : %s\n", msg.from, msg.msg);
                respose.type = CHAT_MSG;
                char buff[5][1024] = {0};
                strcpy(buff[0] , "Nice to meet you!");
                strcpy(buff[1] , "Happy to recieve your message!");
                strcpy(buff[2] , "It's too late, I'm going to sleep!");
                strcpy(buff[3] , "Good night!");
                sprintf(buff[4], "hello [%s], I got your message!", msg.from);
                int n = rand()%5;
                strcpy(respose.msg, buff[n]);
                printf("rand() %% 5 = %d msg : %s\n", n, respose.msg);
                send(fd, &respose, sizeof(respose), 0); 

            } else {
                respose.type = CHAT_MSG;
                char t[30];
                sprintf(t, "UNKNOW MSG_TYPE [0X%X]", msg.type);
                strcpy(respose.msg, t);
                DBG(RED"[0X%x]Unknow message type!\n"NONE, msg.type);
                send(fd, &respose, sizeof(respose), 0); 
            }
        } 
    }     
}
