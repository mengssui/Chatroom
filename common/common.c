/*************************************************************************
	> File Name: common.c
	> Author: Mengbw
	> Mail: 1711900861@qq.com
	> Created Time: Tue 15 Dec 2020 06:27:47 PM CST
 ************************************************************************/

#include "head.h"
char ans[512] = {0}; //全局变量，解决返回局部变量的一种方式

/*
 * 设置非阻塞
 */
int make_nonblock(int fd) { 
    int flag;

    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }

    flag |= O_NONBLOCK;   
    return fcntl(fd, F_SETFL, O_NONBLOCK); //不能直接用，应该是或上这个标志，加上这个标志
}

/*
 * 设置阻塞
 */
int make_block(int fd) {
    int flag;
    if ((flag = fcntl(fd, F_GETFL)) < 0) {
        return -1;
    }
    flag &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flag);
}

/*
 * 在path文件夹中根据key关键字获取对应的内容
 */
char *get_value(const char *path, const char *key) {
    FILE *fp = NULL;
    char *line = NULL, *sub = NULL;
    ssize_t nred, len;  //getline读取包含换行符
    
    if (path == NULL || key == NULL) {   //错误检查
        fprintf(stderr, "Error in argument!\n");
        exit(1);
    }

    if ((fp = fopen(path, "r")) == NULL) { //fopen自己会设置error
        perror("fopen");
        exit(1);
        //return NULL;
    }

    while ((nred = getline(&line, &len, fp)) != -1) { //自动给line申请空间,为什么要传入指针的地址呢：我的理解是line要指向一个新开辟地址的内存位置
        if ((sub = strstr(line, key)) == NULL) continue; //字符串匹配strstr, 找到第一个字符串
        if (line[strlen(key)] == '=' && sub == line) {
            //printf("Find\n");
            strcpy(ans, line + strlen(key) + 1);
            ans[strlen(ans)-1] = 0; //干掉回车符号
        }

    }
    free(line); //释放line分配的空间
    fclose(fp); 
    return ans; //返回局部变量的问题
}
