 ///
 /// @file    socket.h
 /// @author  yangwenhao
 /// @date    2017-05-12 19:31:21
 ///
 
#ifndef __SOCKET_H__	
#define __SOCKET_H__	

#include "Noncopyable.h"

namespace sh
{
class InetAddress;

class Socket : Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();

	void shutdownWrite();//将写端关闭
	int fd() const{ return _sockfd; };
	void nonblock();//设置非阻塞
	static InetAddress getLocalAddr(int sockfd);//获取套接字关联的本地协议地址
	static InetAddress getPeerAddr(int sockfd);//获取套接字关联的远程协议地址
private:
	int _sockfd;
};

}// end of namespace sh


#endif
