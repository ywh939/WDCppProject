 ///
 /// @file    EpollPoller.h
 /// @author  yangwenhao
 /// @date    2017-05-14 14:35:03
 ///
 
#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "MutexLock.h"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <functional>
using std::vector;
using std::map;


namespace sh
{

class Acceptor;
class EpollPoller : Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;
	typedef std::function<void()> Functor;

	EpollPoller(Acceptor & acceptor);
	~EpollPoller();

	void loop();
	void unloop();
	void runInLoop(const Functor & cb);
	void doPendingFunctors();

	void wakeup();
	void handleRead();

	void setConnectionCallback(EpollCallback cb);
	void setMessageCallback(EpollCallback cb);
	void setCloseCallback(EpollCallback cb);
	
private:
	void waitEpollfd();
	void handleConnection();
	void handleMessage(int peerfd);

private:
	Acceptor & _acceptor;
	int _epollfd;
	int _listenfd;
	int _eventfd;
	bool _isLooping;

	MutexLock _mutex;
	vector<Functor> _pendingFunctors;

	vector<struct epoll_event> _eventsList;

	typedef map<int, TcpConnectionPtr> ConnectionMap;
	ConnectionMap _connMap;

	EpollCallback _onConnectionCb;
	EpollCallback _onMessageCb;
	EpollCallback _onCloseCb;
};

}// end of namespace sh


#endif
