 ///
 /// @file    Inetaddress.h
 /// @author  yangwenhao
 /// @date    2017-05-12 19:39:26
 ///
 
#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>//struct sockaddr_in 的头文件
#include <string>
using std::string;

namespace sh
{

class InetAddress
{
public:
	InetAddress(short port);//只提供端口号,系统提供IP地址
	InetAddress(const char * pIp, short port);
	InetAddress(const struct sockaddr_in & addr);

	const struct sockaddr_in * getSockAddrPtr() const;
	string ip() const;
	unsigned short port() const;
private:
	struct sockaddr_in _addr;
};

}// end of namespace sh


#endif
