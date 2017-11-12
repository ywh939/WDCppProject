 ///
 /// @file    TcpConnection.h
 /// @author  yangwenhao
 /// @date    2017-05-12 20:08:42
 ///
 
#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <string>
#include <memory>
#include <functional>
using std::string;

namespace sh
{
class EpollPoller;
class TcpConnection;
//定义一个智能指针,可以复制,传递效率也高
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

class TcpConnection
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::function<void(const TcpConnectionPtr &)> TcpConnectionCallback;

	TcpConnection(int sockfd, EpollPoller * loop);
	~TcpConnection();

	string receive();
	void send(const string & msg);
	void sendAndClose(const string & msg);
	void sendInLoop(const string & msg);
	void shutdown();
	string toString();

	void setConnectionCallback(TcpConnectionCallback cb);
	void setMessageCallback(TcpConnectionCallback cb);
	void setCloseCallback(TcpConnectionCallback cb);

	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();

private:
	Socket _sockfd;
	SocketIO _sockIO;
	const InetAddress _localAddr;
	const InetAddress _peerAddr;
	bool _isShutdownWrite;
	EpollPoller * _loop;

	TcpConnectionCallback _onConnectionCb;
	TcpConnectionCallback _onMessageCb;
	TcpConnectionCallback _onCloseCb;
};

}// end of namespace sh


#endif
