#include "func.h"

void make_server(int sfd,int num,int epfd,pctrl pc)
{	
	struct epoll_event *evs;
	evs=(struct epoll_event*)calloc(num+1,sizeof(struct epoll_event));
	
	int i,j,ret,new_fd;
	short flag;
	while(1)
	{
		bzero(evs,(num+1)*sizeof(struct epoll_event));
		ret=epoll_wait(epfd,evs,num+1,-1);
		for(i=0;i<ret;i++)
		{
			if(sfd==evs[i].data.fd)
			{
				new_fd=accept(sfd,NULL,NULL);
				for(j=0;j<num;j++)
				{
					if(0==pc[j].stat)
					{
						printf("child proccess %d will busy\n",pc[j].pid);
						send_fd(pc[j].fdw,new_fd);
						pc[j].stat=1;
						break;
					}
				}
				close(new_fd);
			}
			for(j=0;j<num;j++)
			{
				if(pc[j].fdw==evs[i].data.fd)//写成等号导致3个进程都打印not budy
				{
					read(pc[j].fdw,&flag,sizeof(flag));
					printf("child process %d is not busy\n",pc[j].pid);
					pc[j].stat=0;
				}
			}
		}
	}
}
void get_conf_msg(char* conf_name,char* ip,int* port,int* num)
{
	int fd=open(conf_name,O_RDWR);
	if(-1==fd)
	{
		perror("open");
		return;
	}
	char buf[100]={0};
	read(fd,buf,sizeof(buf));
	int i,len;
	char buf1[20]={0},buf2[20]={0},buf3[20]={0};
	len=3;
	for(i=0;buf[i+len]!='\n';i++)
	{
		buf1[i]=buf[i+len];
	}
	len=len+1+strlen(buf1)+5;
	for(i=0;buf[i+len]!='\n';i++)
	{
		buf2[i]=buf[i+len];
	}
	len=len+1+strlen(buf2)+4;
	for(i=0;buf[i+len]!='\n';i++)
	{
		buf3[i]=buf[i+len];
	}
	strcpy(ip,buf1);
	*port=atoi(buf2);
	*num=atoi(buf3);
}
int main(int argc,char* argv[])
{
	if(argc!=2)
	{
		printf("error args\n");
		return -1;
	}
	int port,num;
	char ip[20]={0};
	get_conf_msg(argv[1],ip,&port,&num);
	int sfd,epfd;
	pctrl pc=(pctrl)calloc(num,sizeof(ctrl));
	make_child(pc,num);
	make_socket(&sfd,ip,port,num);
	make_epoll(&epfd,pc,sfd,num);
	make_server(sfd,num,epfd,pc);
}
