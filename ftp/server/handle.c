#include "func.h"

void ls_path(int new_fd,char* path)
{
	DIR *dir;
	dir=opendir(path);
	if(NULL==dir)
	{
		perror("opendir");
		return;
	}
	char data_name[100]={0};
	char file_name[20]={0};
	struct dirent *p;
	train t;
	int ret;
	while(NULL!=(p=readdir(dir)))
	{
		bzero(file_name,sizeof(file_name));
		strcpy(file_name,p->d_name);
		if('.'==file_name[0])
		{
		}else{
			bzero(data_name,sizeof(data_name));
			bzero(&t,sizeof(t));
			get_stat(data_name,path,p->d_name);
			t.len=strlen(data_name);
			strcpy(t.buf,data_name);
			ret=send(new_fd,&t,sizeof(t.len)+t.len,0);
		}
	}
	t.len=0;
	send(new_fd,&t.len,sizeof(t.len),0);
}
void get_order_name(char* buf,char* name,int len)
{
	int i,j;
	char str[N];
	bzero(str,sizeof(str));
	for(i=len,j=0;buf[i]!='\0';i++)
	{
		if(buf[i]!=' ')
		{
			str[j]=buf[i];
			j++;
		}
	}
	strcpy(name,str);
}
void hand_request(int new_fd)
{
	hand_enter(new_fd);
	int ret,len;
	char buf[N]={0};
	char path[50]={0};
   	char order_name[10]={0};
	char ip[20]={0};
	int port;
	time_t t;
	
	time(&t);
	
	recv(new_fd,&len,sizeof(len),0);
	recv(new_fd,ip,len,0);
	recv(new_fd,&port,sizeof(port),0);
	insert_mysql_logdata(ip,port,buf,ctime(&t));
	
	while(1)
	{		
		bzero(buf,sizeof(buf));
		recv(new_fd,&len,sizeof(len),0);
		ret=recv(new_fd,buf,len,0);
		
		time(&t);

	//	recv(new_fd,&port,sizeof(port),0);
	//	bzero(ip,sizeof(ip));
	//	recv(new_fd,&len,sizeof(len),0);
	//	recv(new_fd,ip,len,0);
		insert_mysql_logdata(ip,port,buf,ctime(&t));
		
		if(ret>0)
		{
			if(!strncmp(buf,"pwd",3))
			{
				bzero(path,sizeof(path));
				strcpy(path,getcwd(NULL,0));
				send(new_fd,path,sizeof(path),0);
			}
			if(!strncmp(buf,"cd",2))
			{
				bzero(path,sizeof(path));
				get_order_name(buf,order_name,2);
				chdir(order_name);
				strcpy(path,getcwd(NULL,0));
				send(new_fd,path,strlen(path),0);
			}
			if(!strncmp(buf,"ls",2))
			{
				ls_path(new_fd,path);
			}
			if(!strncmp(buf,"puts",4))
			{
				bzero(order_name,sizeof(order_name));
				get_order_name(buf,order_name,4);
				recv_file_from_client(order_name,new_fd);
			}
			if(!strncmp(buf,"gets",4))
			{
					bzero(order_name,sizeof(order_name));
				get_order_name(buf,order_name,4);
				send_file_to_client(order_name,new_fd);
			}
			if(!strncmp(buf,"remove",6))
			{
				bzero(order_name,sizeof(order_name));
				get_order_name(buf,order_name,6);
				remove(order_name);
				bzero(buf,sizeof(buf));
				sprintf(buf,"%s %s %s","remove",order_name,"success");
				send(new_fd,buf,strlen(buf),0);
			}
			else
			{
			}
		}else{
			goto end;
		}
	}
end:
	printf("client close\n");	
	close(new_fd);
}	
