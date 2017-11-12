#include "func.h"

void hand_login(int new_fd)
{
	int i=0,j=0;
	train t;
	bzero(&t,sizeof(t));
	recv(new_fd,&t.len,sizeof(t.len),0);	//接收账户名
	recv(new_fd,t.buf,t.len,0);
	char account[20]={0};
	char salt[20]={0};
	char passward[512]={0};
	query_mysql(&i,t.buf,account,salt,passward);//如果账户名匹配成功，便从数据库中获取已注册的账户名，salt，密码,返回i=1
	if(1==i)
	{
		send(new_fd,&i,sizeof(i),0);	//通知客户端账户正确
		send(new_fd,salt,strlen(salt),0);			//发送salt
		bzero(&t,sizeof(t));
		recv(new_fd,&t.len,sizeof(t.len),0);//接收密码
		recv(new_fd,t.buf,t.len,0);
		if(0==strcmp(t.buf,passward))
		{
			j=1;
			send(new_fd,&j,sizeof(j),0);//通知客户端密码正确
		}else{
			send(new_fd,&j,sizeof(j),0);//通知客户端密码错误
		}
	}else{
		send(new_fd,&i,sizeof(i),0);   //通知客户端账户错误
	}
}
void get_rand_str(char* s,int num)
{
	char* str="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz,./;\"'<>?";
	int i,lstr;
	char ss[2]={0};
	lstr=strlen(str);
	srand((unsigned int)time((time_t*)NULL));
	for(i=1;i<=num;i++)
	{
		sprintf(ss,"%c",str[(rand()%lstr)]);
		strcat(s,ss);
	}
}
void hand_register(int new_fd)
{
	train t;
	bzero(&t,sizeof(t));
	recv(new_fd,&t.len,sizeof(t.len),0);	//接收账户名
	recv(new_fd,t.buf,t.len,0);
	int i=0;
	char account[20]={0};
	char salt[20]={0};
	char passward[512]={0};
	query_mysql(&i,t.buf,account,salt,passward);//如果账户名匹配成功，便从数据库中获取已注册的账户名，salt，密码,返回i=1
	char s[9]={0};
	if(0==i)
	{
		send(new_fd,&i,sizeof(i),0);	//通知客户端数据库没有该账户,可以进行注册
		bzero(account,sizeof(account));
		bzero(salt,sizeof(salt));
		strcpy(account,t.buf);
		get_rand_str(s,8);
		strcpy(salt,"$6$");
		strcat(salt,s);
		send(new_fd,salt,11,0);			//发送salt
		bzero(&t,sizeof(t));
		recv(new_fd,&t.len,sizeof(t.len),0);//接收密码
		recv(new_fd,t.buf,t.len,0);
		insert_mysql_ftpdata(account,salt,t.buf);  //将账户，salt,密码写入数据库
	}else{
		send(new_fd,&i,sizeof(i),0);	//通知客户端数据库已有账户，注册非法
	}
}
void  hand_enter(int new_fd)
{
	int len,ret;
	while(1)
	{
		ret=recv(new_fd,&len,sizeof(len),0);
		if(0==len)
		{
			break;
		}
		if(1==len)
		{
			hand_register(new_fd);
		}	
		if(2==len)
		{
			hand_login(new_fd);
		}
		if(0==ret)
		{
			break;
		}
	}
}
