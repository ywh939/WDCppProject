#include "func.h"

void make_epoll(int* epfd,int sfd)
{
	*epfd=epoll_create(1);
	struct epoll_event evt;
	bzero(&evt,sizeof(evt));
	evt.events=EPOLLIN;
	evt.data.fd=sfd;
	epoll_ctl(*epfd,EPOLL_CTL_ADD,sfd,&evt);
	bzero(&evt,sizeof(evt));
	evt.events=EPOLLIN;
	evt.data.fd=STDIN_FILENO;
	epoll_ctl(*epfd,EPOLL_CTL_ADD,STDIN_FILENO,&evt);
}

void make_socket(int* sfd,char* ip,int port)
{
	*sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==*sfd)
	{
		perror("socket");
		return;
	}
	struct sockaddr_in sev;
	bzero(&sev,sizeof(sev));
	sev.sin_family=AF_INET;
	sev.sin_port=htons(port);
	sev.sin_addr.s_addr=inet_addr(ip);
	int ret;
	ret=connect(*sfd,(struct sockaddr*)&sev,sizeof(sev));
	if(-1==ret)
	{
		perror("connect");
		return;
	}
}
