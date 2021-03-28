/*************************************************************************
	> File Name: wechat.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Wed 30 Dec 2020 06:27:29 PM CST
************************************************************************/
#include "./common/head.h"
#include "./common/wechat.h"
#define MAX 10     //最大的多路复用事件数量

//全局变量
char *config = "./chat.conf";
char name[20] = {0};
char msg[512] = {0};
int port = 0;
struct User *users; //用户数组
int subfd; //子反应堆


//推出时候捕捉ctrl+C信号
void logout(int signum) {
    struct Msg l_msg;
    l_msg.type = CHAT_FIN;
    strcpy(l_msg.from, name);
    strcpy(l_msg.msg, "Bye");
    for (int i = 0; i < MAXUSER; i++) {
        if (users[i].flag & FL_ONLINE) {
            send(users[i].fd, &l_msg, sizeof(msg), 0);
        }
    }
    DBG(RED"\n💨💤LogOut from wechat system…\n"NONE);
    exit(0);
}

int main (int argc, char** argv) {
    int opt;
    int listener, new_fd;

    //命令解析
    while ((opt = getopt(argc, argv, "p:n:m:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'n':
                strcpy(name, optarg);
                break;
            case 'm':
                strcpy(msg, optarg);
                break;
            default:
                fprintf(stderr, RED"Usage : %s -n name -p port -m msg\n"NONE, argv[0]);
                exit(1);
        }
    }

    //注册信号
    signal(SIGINT, logout);

    //没有传入参数 : 读取配置文件
    if (!strlen(name)) strncpy(name, get_value(config, "name"), 20);
    if (!strlen(msg)) strncpy(msg, get_value(config, "msg"), sizeof(msg));
    if (!port) port = atoi(get_value(config, "port"));
    DBG(BLUE"<1>Geting config file! "NONE"Name = %s, Port = %d, Msg = %s\n", name, port, msg);

    //中文支持
    setlocale(LC_ALL, "");

    //创建udp套接字,绑定了套接字和端口
    if ((listener = socket_create_udp(port)) < 0) {
        perror("socket_create_udp");
        exit(1);
    }
    
    //创建IO多路复用,相当主反应堆
    int epollfd;
    if ((epollfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }

    //创建subfd子反应堆套接字
    if ((subfd = epoll_create(1)) < 0) {
        perror("epoll_create");
        exit(1);
    }

    struct epoll_event ev, events[MAX];
    //创建用户的数组列表
    users = (struct User *)calloc(MAXUSER, sizeof(struct User)); 
    ev.events = EPOLLIN;   //监听信号类型：信号来
    ev.data.fd = listener; //主监听

    //将listener加入epoll
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        perror("epoll_ctl");
        exit(1);
    }

    //创建客户发现线程,开始向所有iplis中的用户发送SYN发现包
    pthread_t c_discover;  //客户发现
    pthread_create(&c_discover, NULL, client_discover, NULL);
    DBG(BLUE"<2>User Find thread!"NONE"\n");

    //心跳线程
    pthread_t heart_beat_tid;
    pthread_create(&heart_beat_tid, NULL, heart_beat, NULL);
    DBG(BLUE"<3>Heart Beat thread!"NONE"\n");

    //子反应堆线程
    pthread_t reactor_tid;
    pthread_create(&reactor_tid, NULL, reactor, NULL);
    DBG(BLUE"<4>Sub Reactor thread!"NONE"\n");

    //发送信息线程
    pthread_t send_tid;
    pthread_create(&send_tid, NULL, send_chat, NULL);
    DBG(BLUE"<5>Send Message thread!"NONE"\n\n\n");

    //主反应堆，监听listen套接字
    while (1) {  
        int nfds = epoll_wait(epollfd, events, MAX, -1); //无限期等待
        printf(BLUE"<Main Reactor>"NONE" : Got Events...\n");

        if (nfds < 0) {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < nfds; i++) {
            struct User newuser;
            if ((new_fd = udp_accept(listener, &newuser)) < 0) {
                fprintf(stderr, RED"error in udp_accept!"NONE"\n");
                continue;
            }
            //添加到从反应堆
            add_to_sub_reactor(subfd, &newuser);
        }
    }
    return 0;
}
