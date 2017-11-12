 ///
 /// @file    InerAddress.cc
 /// @author  yangwenhao
 /// @date    2017-05-12 20:21:19
 ///
 
#include "InetAddress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace sh
{

InetAddress::InetAddress(short port)
{
	::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = INADDR_ANY;//系统自动填入本机地址
}

InetAddress::InetAddress(const char * pIp, short port)
{
	::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = inet_addr(pIp);
}

InetAddress::InetAddress(const struct sockaddr_in & addr)
: _addr(addr)
{}

const struct sockaddr_in * InetAddress::getSockAddrPtr() const
{
	return &_addr;
}

string InetAddress::ip() const
{
	return string(inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
	return ntohs(_addr.sin_port);
}

}// end of namespace sh
