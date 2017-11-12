 ///
 /// @file    EpollPoller.cc
 /// @author  yangwenhao
 /// @date    2017-05-14 16:12:25
 ///
 
#include "EpollPoller.h"
#include "SocketUtil.h"
#include "Acceptor.h"
#include <assert.h>

#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

EpollPoller::EpollPoller(Acceptor & acceptor)
: _acceptor(acceptor)
, _epollfd(createEpollFd())
, _listenfd(_acceptor.fd())
, _eventfd(createEventFd())
, _isLooping(false)
, _eventsList(1024)
{
	addEpollFdRead(_epollfd, _listenfd);
	addEpollFdRead(_epollfd, _eventfd);
}

EpollPoller::~EpollPoller()
{
	cout << "~EpollPoller()" << endl;
	::close(_epollfd);
}

void EpollPoller::loop()
{
	_isLooping = true;
	while(_isLooping)
	{
		waitEpollfd();
	}
}

void EpollPoller::unloop()
{
	if(_isLooping)
		_isLooping = false;
}

void EpollPoller::runInLoop(const Functor & cb)		//函数还在子线程
{
	{
		MutexGuard mlg(_mutex);						//当各子线程都要发送时
		_pendingFunctors.push_back(cb);				//先将发送功能放在vector中
	}
	wakeup();										//唤醒event,用于线程间通信(这里cb绑定的是IO线程(主线程)send())
}

void EpollPoller::wakeup()
{
	uint64_t one = 1;
	ssize_t n = ::write(_eventfd, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::wakeup() n != 8");
	}
}

void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	_onConnectionCb = cb;
}

void EpollPoller::setMessageCallback(EpollCallback cb)
{
	_onMessageCb = cb;
}

void EpollPoller::setCloseCallback(EpollCallback cb)
{
	_onCloseCb = cb;
}

void EpollPoller::waitEpollfd()
{
	int nready;
	do{
		nready = ::epoll_wait(_epollfd,
							  &(*_eventsList.begin()),
							  _eventsList.size(),
							  2000);						//设置2s超时
	}while(-1 == nready && errno == EINTR);

	if(-1 == nready)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(0 == nready)
	{
		printf("epoll_wait timeout\n");						//超时
	}
	else
	{
		if(nready == static_cast<int>(_eventsList.size())) 
		{
			_eventsList.resize(_eventsList.size() * 2);		//扩容操作  
		}

		for(int idx = 0; idx != nready; ++idx)
		{
			if(_eventsList[idx].data.fd == _listenfd)
			{
				if(_eventsList[idx].events & EPOLLIN)
				{
					handleConnection();						//处理连接情况
				}
			}
			else if(_eventsList[idx].data.fd == _eventfd)
			{
				if(_eventsList[idx].events & EPOLLIN)
				{
					handleRead();							//处理event,先read,再write才能唤醒event
					doPendingFunctors();
				}
			}
			else
			{
				if(_eventsList[idx].events & EPOLLIN)
				{
					handleMessage(_eventsList[idx].data.fd);//处理某一个客户端的发送来的数据
				}
			}
		}// end for
	}// end else
}

void EpollPoller::handleConnection()
{
	int peerfd = _acceptor.accept();						//获取listen后的sockfd
	addEpollFdRead(_epollfd, peerfd);						//定义在SokctUtil.h

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));	
	conn->setConnectionCallback(_onConnectionCb);
	conn->setMessageCallback(_onMessageCb);
	conn->setCloseCallback(_onCloseCb);

	std::pair<ConnectionMap::iterator, bool> ret;			//设置insert的返回类型
	ret = _connMap.insert(std::make_pair(peerfd, conn));	//将监听到的每个sockfd与TcpConnection写入map

	assert(ret.second == true);								//判断是否insert成功
	(void)ret;												//消除waring的影响

	conn->handleConnectionCallback();						//调用SpellCorrectServer的onConnection()
}

void EpollPoller::handleRead()
{
	uint64_t one = 1;
	ssize_t n = ::read(_eventfd, &one, sizeof(one));
	if(n != sizeof(one))
	{
		perror("EpollPoller::hanbdleRead() n != 8");
	}
}

void EpollPoller::doPendingFunctors()
{
	vector<Functor> functors;				//这里使用局部变量,将不影响IO线程与子线程同时执行functor,存functor
	{
		MutexGuard mlg(_mutex);				//但是当进行交换的时候,不希望在这个时候有发送功能
		functors.swap(_pendingFunctors);	//添加到共享资源vector中, 故对共享资源加锁
	}

	for(size_t i = 0; i< functors.size(); ++i)
	{
		functors[i]();						//执行发送功能,将服务器处理的数据发给客户端
	}
}

void EpollPoller::handleMessage(int peerfd)
{
	bool isClosed = isConnectionClosed(peerfd);				//定义在SocketUtil.h,判断客户端是否关闭
	ConnectionMap::iterator it = _connMap.find(peerfd);
	assert(it != _connMap.end());							//如果没找到peerfd,终止程序执行

	if(isClosed)
	{
		it->second->handleCloseCallback();					//调用SpellCorrectServer的onClose()
		delEpollReadFd(_epollfd, peerfd);
		_connMap.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();				//调用SpellCorrectServer的onMessage()
	}
}

}// end of namespace sh
