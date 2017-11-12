 ///
 /// @file    TcpConnection.cc
 /// @author  yangwenhao
 /// @date    2017-05-14 10:06:33
 ///
 
#include "TcpConnection.h"
#include "EpollPoller.h"
#include <stdio.h>
#include <string.h>

namespace sh
{

TcpConnection::TcpConnection(int sockfd, EpollPoller * loop)
: _sockfd(sockfd)
, _sockIO(sockfd)
, _localAddr(Socket::getLocalAddr(sockfd))
, _peerAddr(Socket::getPeerAddr(sockfd))
, _isShutdownWrite(false)
, _loop(loop)
{
	_sockfd.nonblock();							//设置为非阻塞
}

TcpConnection::~TcpConnection()
{
	if(!_isShutdownWrite)
	{
		_isShutdownWrite = true;
		shutdown();
	}
}

string TcpConnection::receive()
{
	char buf[65536];
	memset(buf, 0, sizeof(buf));
	size_t ret = _sockIO.readline(buf, sizeof(buf));
	if(0 == ret)
		return string();			//返回空字符串
	else
		return string(buf);
}

void TcpConnection::send(const string & msg)
{
//	printf("发送前msg.size() = %d\n", msg.size());
/*	size_t cnt =*/ _sockIO.writen(msg.c_str(), msg.size());
//	printf("发送成功 %d\n", cnt);
}

//针对php服务器
void TcpConnection::sendAndClose(const string & msg)
{
	send(msg);
	shutdown();
}

void TcpConnection::sendInLoop(const string & msg)
{
	_loop->runInLoop(bind(&TcpConnection::sendAndClose, this, msg));//绑定IO线程(主线程)发送功能
}

void TcpConnection::shutdown()
{
	if(!_isShutdownWrite)
		_sockfd.shutdownWrite();
	_isShutdownWrite = true;
}

string TcpConnection::toString()
{
	char str[100];
	snprintf(str, sizeof(str), "%s:%d -> %s:%d",
			_localAddr.ip().c_str(),
			_localAddr.port(),
			_peerAddr.ip().c_str(),
			_peerAddr.port());
	return string(str);
}

void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
	_onConnectionCb = cb;
}

void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
	_onMessageCb = cb;
}

void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
	_onCloseCb = cb;
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnectionCb)
		_onConnectionCb(shared_from_this());
}

void TcpConnection::handleMessageCallback()
{
	if(_onMessageCb)
		_onMessageCb(shared_from_this());
}

void TcpConnection::handleCloseCallback()
{
	if(_onCloseCb)
		_onCloseCb(shared_from_this());
}

}// end of namespace sh
