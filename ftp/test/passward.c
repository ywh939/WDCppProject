#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <shadow.h>
#include <errno.h>
#include <crypt.h>

void help(void)
{
	printf("用户密码验证程序\n第一个参数为用户名\n");
	exit(-1);
}
void error_quit(char* msg)
{
	perror(msg);
	exit(-2);
}
void get_salt(char* salt,char* passwd)
{
	int i,j;
	for(i=0,j=0;passwd[i]&&j!=3;i++)
	{
		if('$'==passwd[i])
		{
			j++;
		}
	}
	strncpy(salt,passwd,i-1);
}
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		help();
	}
	struct spwd* sp;
	char* passwd;
	char salt[512]={0};
	passwd=getpass("请输入密码:");
	if(NULL==((sp=getspnam(argv[1]))))
	{
		error_quit("获取用户名和密码");
	}
	
	get_salt(salt,sp->sp_pwdp);
	printf("salt=%s\n",salt);
	if(0==strcmp(sp->sp_pwdp,crypt(passwd,salt)))
	{
		printf("验证通过!\n");
	}else{
		printf("验证失败!\n");
	}
	return 0;
}
