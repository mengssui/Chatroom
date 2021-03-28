/*************************************************************************
	> File Name: common.h
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 15 Dec 2020 06:27:56 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

int make_nonblock(int fd);  //set block
int make_block(int fd);     //set nonblock

//读取配置文件
char* get_value(const char* file, const char* key);

#endif
