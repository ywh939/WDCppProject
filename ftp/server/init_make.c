#include "func.h"

void make_epoll(int* epfd,pctrl pc,int sfd,int num)
{
	*epfd=epoll_create(1);
	struct epoll_event evt;
	bzero(&evt,sizeof(evt));
	evt.events=EPOLLIN;
	evt.data.fd=sfd;
	epoll_ctl(*epfd,EPOLL_CTL_ADD,sfd,&evt);
	int i;
	for(i=0;i<num;i++)
	{
		evt.events=EPOLLIN;
		evt.data.fd=pc[i].fdw;
		epoll_ctl(*epfd,EPOLL_CTL_ADD,pc[i].fdw,&evt);
	}
}

void make_socket(int* sfd,char* ip,int port,int num)
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
	ret=bind(*sfd,(struct sockaddr*)&sev,sizeof(sev));
	if(-1==ret)
	{
		perror("bind");
		return;
	}
	ret=listen(*sfd,num);
	if(-1==ret)
	{
		perror("listen");
		return;
	}
}

void child_handle(int fdr)
{
	int new_fd;
	short flag=1;
	signal(SIGPIPE,SIG_IGN);
	while(1)
	{
		recv_fd(fdr,&new_fd);
		hand_request(new_fd);
		write(fdr,&flag,sizeof(flag));
	}
}

void make_child(pctrl pc,int child_num)
{
	int i,fds[2];
	int pid;
	for(i=0;i<child_num;i++)
	{
		socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
		pid=fork();
		if(!pid)
		{
			close(fds[1]);
			child_handle(fds[0]);
		}
		close(fds[0]);
		pc[i].pid=pid;
		pc[i].fdw=fds[1];
		pc[i].stat=0;
	}
}
