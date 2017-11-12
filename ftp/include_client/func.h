#ifndef __FUNC_H__
#define __FUNC_H__
#include "head.h"
#include "train.h"

void make_socket(int*,char*,int);
void make_epoll(int*,int);
void enter_func(int,int,char*,int);
void make_client(int,int,char*,int);
void register_func(int,int,char*,int);
void login_func(int,int,char*,int);
void upload_file(int,char*);
void download_file(int);
void get_order_name(char*,char*,int);

#endif

