#include "func.h"

void login_func(int sfd,int epfd,char* ip,int port)
{
	system("clear");
	static int i=0;
	static int j=3;
	i++;
	if(1==i)
	{
		printf("please input account and passward of yourself\n");
	}else{
		printf("error account or passward,please input again\n");
		printf("you have only %d times remain\n",j-i);
	}
	if(0==j-i)
	{
		system("clear");
		exit(0);
	}
	train t;
	bzero(&t,sizeof(t));
	printf("\n\n\taccount:");
	scanf("%s",t.buf);
	char* pd;
	pd=getpass("\tpassward:");
	t.len=2;
	send(sfd,&t.len,sizeof(t.len),0);	//通知服务器要登录了
	t.len=strlen(t.buf);
	send(sfd,&t,sizeof(t.len)+t.len,0);	//发送账户名
	int m,n;
	recv(sfd,&m,sizeof(m),0);	//接收服务器的账户判断
	char salt[20]={0};
	if(1==m)
	{
		recv(sfd,salt,sizeof(salt),0);		//接收salt
		bzero(&t,sizeof(t));
		strcpy(t.buf,crypt(pd,salt));
		t.len=strlen(t.buf);
		send(sfd,&t,sizeof(t.len)+t.len,0);	//发送密码
		recv(sfd,&n,sizeof(n),0);		//接收服务器的密码判断
		if(1==n)
		{
			t.len=0;						//通知服务器已登录
			send(sfd,&t.len,sizeof(t.len),0);
			make_client(sfd,epfd,ip,port);
		}else{
			login_func(sfd,epfd,ip,port);
		}
	}else{
		login_func(sfd,epfd,ip,port);
	}
}
void register_func(int sfd,int epfd,char* ip,int port)
{
	system("clear");
	static int i=0;
	i++;
	if(1!=i)
	{
		printf("account registered or passward error,please register again\n");
	}else{
		printf("please input account and passward of yourself\n");
	}
	train t;
	bzero(&t,sizeof(t));
	printf("\n\n\taccount:");
	scanf("%s",t.buf);
	char pa[10]={0};
	char pb[10]={0};
	memcpy(pa,getpass("\tpassward:"),10);
	memcpy(pb,getpass("\tpassward:"),10);
	int j=0;
	if(0==strcmp(pa,pb))
	{
		t.len=1;
		send(sfd,&t.len,sizeof(t.len),0);	//通知服务器要注册了
		t.len=strlen(t.buf);
		send(sfd,&t,sizeof(t.len)+t.len,0);	//发送账户名
		recv(sfd,&j,sizeof(j),0);		//接收服务器账户判断
		if(0==j)
		{
			char salt[20]={0};
			recv(sfd,salt,sizeof(salt),0);		//接收salt
			bzero(&t,sizeof(t));
			strcpy(t.buf,crypt(pa,salt));
		 	t.len=strlen(t.buf);
			send(sfd,&t,sizeof(t.len)+t.len,0);	//发送密码
			printf("t.len=%d,t.buf=%s\n",t.len,t.buf);
			enter_func(sfd,epfd,ip,port);
		}else{
			register_func(sfd,epfd,ip,port);
		}
	}else{
		register_func(sfd,epfd,ip,port);
	}
}
void enter_func(int sfd,int epfd,char* ip,int port)
{
	system("clear");
	static int i=0;
	i++;
	if(i!=1)
	{
		printf("register success\n");
	}
	printf("\n\n\t1.register\n\t2.log in\n");
	printf("\nplease select number:");
	int n;
	scanf("%d",&n);
	switch(n)
	{
		case 1:register_func(sfd,epfd,ip,port);break;
		case 2:login_func(sfd,epfd,ip,port);break;
		default:enter_func(sfd,epfd,ip,port);
	}
}
