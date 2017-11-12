 ///
 /// @file    SocketUtil.h
 /// @author  yangwenhao
 /// @date    2017-05-12 20:55:13
 ///
 
#ifndef __SOCKETUTIL_H__
#define __SOCKETUTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

namespace sh
{

inline int createSocketFd()
{
	int fd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd)
	{
		perror("socket create error!");	
	}
	return fd;
}

//将read()设置为非阻塞
inline void setNonblock(int fd)
{
	int flags = ::fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	::fcntl(fd, F_SETFL, flags);
}

//预览内核态缓冲区的数据,并不将数据移出内核态
inline size_t recvPeek(int sockfd, void * buf, size_t len)
{
	int nread;
	do
	{
		nread = ::recv(sockfd, buf, len, MSG_PEEK);
	}while(nread == -1 && errno == EINTR);//<erron.h>头文件,表示信号中断
	return nread;
}

//通过预览数据，判断conn是否关闭
inline bool isConnectionClosed(int sockfd)
{
	char buf[1024];
	int nread = recvPeek(sockfd, buf, sizeof(buf));
	if(-1 == nread)
	{
		perror("recvPeek--- ");
		return true;
	}
	return (0 == nread);
}

inline int createEpollFd()
{
	int efd = ::epoll_create1(0);
	if(-1 == efd)
	{
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return efd;
}

inline void addEpollFdRead(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl add error");
		exit(EXIT_FAILURE);
	}
}

inline void delEpollReadFd(int efd, int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
	if(-1 == ret)
	{
		perror("epoll_ctl del error");
		exit(EXIT_FAILURE);
	}
}

inline int createEventFd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if(-1 == evtfd)
	{
		perror("eventfd create error");
	}
	return evtfd;
}

}// end of namespace sh

#endif
