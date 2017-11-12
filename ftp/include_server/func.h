#ifndef __FUNC_H__
#define __FUNC_H__
#include "head.h"
#include "train.h"

void make_server(int,int,int,pctrl);
void get_conf_msg(char*,char*,int*,int*);
void make_epoll(int*,pctrl,int,int);
void make_socket(int*,char*,int,int);
void child_handle(int);
void make_child(pctrl,int);
void send_fd(int,int);
void recv_fd(int,int*);
void recv_file_from_client(char*,int);
void send_file_to_client(char*,int);
void ls_path(int,char*);
void stat_mode(char*,int);
void get_stat(char*,char*,char*);
void get_order_name(char*,char*,int);
void hand_request(int);
void hand_enter(int);
void hand_register(int);
void get_rand_str(char*,int);
void insert_mysql_ftpdata(char*,char*,char*);
void hand_login(int);
void query_mysql(int*,char*,char*,char*,char*);
void insert_mysql_logdata(char*,int,char*,char*);
void updata_mysql_logdata(char*);

#endif

