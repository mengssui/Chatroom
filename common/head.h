/*************************************************************************
	> File Name: head.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 24 Nov 2020 07:19:08 PM CST
 ************************************************************************/
#ifndef _HEAD_H
#define _HEAD_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <locale.h>
#include <ncurses.h>

#include "color.h"
#include "tcp_client.h"
#include "tcp_server.h"
#include "common.h"
#include "wechat.h"
#include "udp_server.h"
#include "udp_client.h"
#include "reactor.h"
#include "client_discover.h"
#include "heart_beat.h"
#include "send_chat.h"
#include "chat_ui.h"

//定义了一个宏
#ifdef DEBUG
#define DBG(fmt, args...) printf(fmt,##args) 
#else
#define DBG(fmt, args...) 
#endif

#endif
