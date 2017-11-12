 ///
 /// @file    SocketIO.cc
 /// @author  yangwenhao
 /// @date    2017-05-13 17:24:53
 ///
 
#include "SocketIO.h"
#include "SocketUtil.h"

namespace sh
{

SocketIO::SocketIO(int sockfd)
: _sockfd(sockfd)
{}

size_t SocketIO::readn(char * buf, size_t count)
{
	size_t nleft = count;						//为了返回接收到的数据数目
	char * pbuf = buf;
	while(nleft > 0)
	{
		int nread = ::read(_sockfd, pbuf, nleft);
		if(-1 == nread)
		{
			if(errno == EINTR)
				continue;						//忽略掉中断信号
			return EXIT_FAILURE;
		}
		else if(0 == nread)
		{
			break;
		}
		pbuf += nread;
		nleft -= nread;
	}
	return (count - nleft);
}

size_t SocketIO::writen(const char * buf, size_t count)
{
	size_t nleft = count;
	const char * pbuf = buf;
	while(nleft > 0)
	{
		int nwrite = ::write(_sockfd, pbuf, nleft);
		if(-1 == nwrite)
		{
			if(errno == EINTR)
				continue;
			return EXIT_FAILURE;
		}
		nleft -= nwrite;
		pbuf += nwrite;
	}
	return (count - nleft);
}

size_t SocketIO::recv_peek(char * buf, size_t count)
{
	int nread;
	do
	{
		nread = ::recv(_sockfd, buf, count, MSG_PEEK);
	}while(-1 == nread && errno == EINTR);
	return nread;
}

size_t SocketIO::readline(char * pbuf, size_t maxlen)
{
	size_t nleft = maxlen - 1;
//	char * pbuf = buf;
	size_t total = 0;
	while(nleft > 0)
	{
		int nread = recv_peek(pbuf, nleft);				//先查看内核态中的nleft个数据,并不拷贝
		if(nread <= 0)
			return nread;
		for(size_t idx = 0; idx != nread; ++idx)		//通过检查'\n',读取一行 
		{
			if('\n' == pbuf[idx])	
			{
				size_t nsize = idx + 1;					//从pbuf[0]到pbuf[idx]的长度
				if(readn(pbuf, nsize) != nsize)
					return EXIT_FAILURE;
				pbuf += nsize;
				total += nsize;
				*pbuf = 0;								//将从内核态拷贝到用户态的一行末尾设置'\0',
				return total;
			}
		}
		if(readn(pbuf, nread) != nread)
			return EXIT_FAILURE;
		pbuf += nread;
		nleft -= nread;
		total += nread;
	}
	*pbuf = 0;
	return maxlen - 1;
}

}// end of namespace sh
