 ///
 /// @file    Acceptor.cc
 /// @author  yangwenhao
 /// @date    2017-05-12 22:50:35
 ///
 
#include "Acceptor.h"
#include "SocketUtil.h"

#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

//在Acceptor构造函数里将参数传入两个对象中(初始化成员变量)
Acceptor::Acceptor(int listenfd, const InetAddress & addr)
: _listenSock(listenfd)
, _addr(addr)
{}

void Acceptor::ready()
{
	setReuseAddr(true);						//重置端口号和ip,可以重复使用一个端口号和ip
	setReusePort(true);
	bind();									//将ip和端口号绑定
	listen();								//使服务器的ip和端口号处于监听状态
}

int Acceptor::accept()
{
	int peerfd = ::accept(_listenSock.fd(), NULL, NULL);
	if(-1 == peerfd)
	{
		perror("accept error");
	}
	return peerfd;
}

int Acceptor::fd() const
{
	return _listenSock.fd();
}

//closesocket(一般不会立即关闭而经历TIME_WAIT)后想继续重用该socket
void Acceptor::setReuseAddr(bool flag)
{
	int on = (flag ? 1 : 0);
	if(-1 == ::setsockopt(_listenSock.fd(),//指向一个打开的套接口描述字
					SOL_SOCKET,//基本套接口
					SO_REUSEADDR,//允许重用本地地址
					&on,
					static_cast<socklen_t>(sizeof(on))))
	{
		perror("setsockopt reuseaddr error");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);//EXIT_FAILURE是stdlib.h中定义的一个符号常量
							//作为exit()的参数使用,表示没有成功地执行一个程序
	}
}

void Acceptor::setReusePort(bool flag)
{
#ifdef SO_REUSEPORT
	int on = (flag ? 1 : 0);
	if(-1 == ::setsockopt(_listenSock.fd(),
					SOL_SOCKET,
					SO_REUSEPORT,
					&on,
					static_cast<socklen_t>(sizeof(on))))
	{
		perror("setsockopt reuseport error");	
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
#else
	if(flag)
	{
		//stderr,标准错误,无缓冲直接输出
		fprintf(stderr, "SO_REUSEADDR is not supported!\n");
	}
#endif
}

void Acceptor::bind()
{
	if(-1 == ::bind(_listenSock.fd(),
					(const struct sockaddr*)_addr.getSockAddrPtr(),
					sizeof(InetAddress)))
	{
		perror("bind error!");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

void Acceptor::listen()
{
	if(-1 == ::listen(_listenSock.fd(), 10))
	{
		perror("listen error!");
		::close(_listenSock.fd());
		exit(EXIT_FAILURE);
	}
}

}// end of namespace sh
