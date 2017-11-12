 ///
 /// @file    TcpServer.cc
 /// @author  yangwenhao
 /// @date    2017-05-14 21:56:31
 ///
 
#include "TcpServer.h"
#include "InetAddress.h"
#include "SocketUtil.h"
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

TcpServer::TcpServer(const string & ip, unsigned short port)
: _acceptor(createSocketFd(), InetAddress(ip.c_str(), port))
, _poller(_acceptor)
{}

void TcpServer::start()
{
	_acceptor.ready();										//使服务器ip和端口号处于监听状态
	_poller.setConnectionCallback(_connectionCallback);		
	_poller.setMessageCallback(_messageCallback);
	_poller.setCloseCallback(_closeCallback);
	_poller.loop();											//是服务器处于epoll监听
}

void TcpServer::stop()
{
	_poller.unloop();
}

void TcpServer::setConnectionCallback(TcpServerCallback cb)
{
	_connectionCallback = cb;
}

void TcpServer::setMessageCallback(TcpServerCallback cb)
{
	_messageCallback = cb;
}

void TcpServer::setCloseCallback(TcpServerCallback cb)
{
	_closeCallback = cb;
}

}// end of namespace sh
