/*************************************************************************
	> File Name: chat_ui.h
	> Author: suyelu
	> Mail: suyelu@126.com
	> Created Time: Sun 23 Aug 2020 02:07:23 PM CST
 ************************************************************************/

#ifndef _CHAT_UI_H
#define _CHAT_UI_H
#define MSG_WIDTH 100
#define MSG_HEIGHT 25
#define HELP_WIDTH 15
#define INPUT_HEIGHT 5

void gotoxy(int x, int y);
void gotoxy_putc(int x, int y, char c);
void gotoxy_puts(int x, int y, char *s);
void w_gotoxy_putc(WINDOW *win, int x, int y, char c);
void w_gotoxy_puts(WINDOW *win, int x, int y, char *s);
void init_ui();
void print_message(WINDOW *win, struct Msg *msg, int type);
#endif
