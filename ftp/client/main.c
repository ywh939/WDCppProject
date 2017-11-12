#include "func.h"

void make_client(int sfd,int epfd,char* ip,int port)
{
	system("clear");
	printf("please input command\n");
	struct epoll_event evs[2];
	train t;
	bzero(&t,sizeof(t));
	int i,ret;
	int len;
	char buf[100]={0};
	char order_name[10]={0};

	t.len=strlen(ip);
	strcpy(t.buf,ip);
	send(sfd,&t,sizeof(t.len)+t.len,0);
	send(sfd,&port,sizeof(port),0);

	while(1)
	{
		bzero(evs,sizeof(evs));
		ret=epoll_wait(epfd,evs,2,-1);	
		for(i=0;i<ret;i++)
		{
			if(EPOLLIN==evs[i].events && STDIN_FILENO==evs[i].data.fd)
			{
				system("clear");
				bzero(buf,sizeof(buf));
				read(STDIN_FILENO,buf,sizeof(buf));
				bzero(&t,sizeof(t));
				t.len=strlen(buf)-1;
				strcpy(t.buf,buf);
				send(sfd,&t,sizeof(t.len)+t.len,0);
				
			//	send(sfd,&port,sizeof(port),0);
			//	bzero(&t,sizeof(t));
			//	t.len=strlen(ip);
			//	strcpy(t.buf,ip);
			//	printf("len=%d,t.buf=%s\n",t.len,t.buf);
			//	send(sfd,&t,sizeof(t.len)+t.len,0);

				if(!strncmp(buf,"puts",4))
				{
					bzero(order_name,sizeof(order_name));
					get_order_name(buf,order_name,4);
					upload_file(sfd,order_name);
				}
			}
			if(sfd==evs[i].data.fd)
			{
				if(!strncmp(buf,"gets",4))
				{
					download_file(sfd);
					break;
				}
				if(!strncmp(buf,"ls",2))
				{
					while(1)
					{
						ret=recv(sfd,&len,sizeof(len),0);
						if(len>0)
						{
							bzero(buf,sizeof(buf));
							ret=recv(sfd,buf,len,0);
							printf("%s\n",buf);
						}else{
							break;
						}
					}
					break;
				}
				if(!strncmp(buf,"puts",4))
				{
					recv(sfd,&len,sizeof(len),0);
					if(0==len)
					{
						printf("upload %s success\n",order_name);
					}
				}
				else
				{
					bzero(buf,sizeof(buf));
					ret=recv(sfd,buf,sizeof(buf),0);
					printf("%s\n",buf);
				}
			}
		}
	}
}


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		perror("error args\n");
		return -1;
	}
	int port=atoi(argv[2]);
	char ip[20]={0};
	strcpy(ip,argv[1]);
	int sfd,epfd;
	make_socket(&sfd,ip,port);
	make_epoll(&epfd,sfd);
	enter_func(sfd,epfd,ip,port);
}
