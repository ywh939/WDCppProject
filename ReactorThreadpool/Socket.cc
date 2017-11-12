 ///
 /// @file    Socket.cc
 /// @author  yangwenhao
 /// @date    2017-05-12 20:49:42
 ///
 
#include "Socket.h"
#include "SocketUtil.h"
#include "InetAddress.h"

namespace sh
{

Socket::Socket(int sockfd)
: _sockfd(sockfd)
{}

Socket::Socket()
: _sockfd(createSocketFd())
{}

Socket::~Socket()
{
	::close(_sockfd);
}

//将socket设置非阻塞,将不会阻塞在read()
void Socket::nonblock()
{
	setNonblock(_sockfd);
}

//shutdown()可用于关闭一个方向的连接，这里将写端关闭
void Socket::shutdownWrite()
{
	if(::shutdown(_sockfd, SHUT_WR) == -1)
	{
		perror("shutdown write error!");
	}
}

//获取套接字关联的本地协议地址
InetAddress Socket::getLocalAddr(int sockfd)
{
	struct sockaddr_in addr;//保存返回的地址结构
	socklen_t len = sizeof(sockaddr_in);
	if(::getsockname(sockfd, (struct sockaddr*)&addr, &len) == -1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);
}

//获取套接字关联的远程协议地址
InetAddress Socket::getPeerAddr(int sockfd)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(sockaddr_in);
	if(::getpeername(sockfd, (struct sockaddr*)&addr, &len) == -1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}

}// end of nemespace sh
