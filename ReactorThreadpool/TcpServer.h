 ///
 /// @file    TcpServer.h
 /// @author  yangwenhao
 /// @date    2017-05-14 21:40:18
 ///
 
#ifndef __TCPSRVER_H__
#define __TCPSRVER_H__

#include "Acceptor.h"
#include "EpollPoller.h"
#include <string>
using std::string;

namespace sh
{

class TcpServer
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;

	TcpServer(const string & ip, unsigned short port);
	TcpServer(unsigned short port);

	void start();
	void stop();
	
	void setConnectionCallback(TcpServerCallback cb);
	void setMessageCallback(TcpServerCallback cb);
	void setCloseCallback(TcpServerCallback cb);
private:
	Acceptor _acceptor;
	EpollPoller _poller;

	TcpServerCallback _connectionCallback;
	TcpServerCallback _messageCallback;
	TcpServerCallback _closeCallback;
};

}// end of namespace sh


#endif
