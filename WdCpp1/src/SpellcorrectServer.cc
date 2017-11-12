 ///
 /// @file    SpellcorrectServer.cc
 /// @author  yangwenhao
 /// @date    2017-05-17 19:48:35
 ///
 
#include "SpellcorrectServer.h"
#include "Task.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using std::bind;
using std::placeholders::_1;

namespace sh
{

SpellcorrectServer::SpellcorrectServer(
		size_t buffsize, 
		size_t threadNum, 
		int	initTime, 
		int intervalTime, 
		int wordNum)
: _conf(Configuration::getConfigurationPtr())
, _buffsize(buffsize)
, _threadNum(threadNum)
, _wordNum(wordNum)
, _threadpool(_buffsize, _threadNum)
, _tcpserver(_conf->query("ip"), stoi(_conf->query("port")))
, _cacheManager(initTime, intervalTime)
{
//	cout << "SpellcorrectServer::SpellcorrectServer()" << endl;
}

void SpellcorrectServer::start()
{
	_tcpserver.setConnectionCallback(bind(&SpellcorrectServer::onConnection, this, _1));
	_tcpserver.setMessageCallback(bind(&SpellcorrectServer::onMessage, this, _1));
	_tcpserver.setCloseCallback(bind(&SpellcorrectServer::onClose, this, _1));

	_threadpool.start();											//启动线程池
	sleep(1);
	_cacheManager.init(_threadpool.getCachePtrMap());				//初始化缓存管理
	_tcpserver.start();												//启动epoll_wait
}

void SpellcorrectServer::onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << endl;								//将连接的客户端ip和端口号打印出来
	conn->send("hello, welcome to Chat Server.\r\n");
}

void SpellcorrectServer::onMessage(const TcpConnectionPtr & conn)
{
	string s(conn->receive());					//调用conn的recevie()用来将服务器接收到的数据从内核态读到用户态
	TaskPtr task(new Task(s, conn, _wordNum));	//创建一个任务来解决msg
	_threadpool.addTask(task);					//将任务添加到buffer中
}

void SpellcorrectServer::onClose(const TcpConnectionPtr & conn)
{
	printf("%s close\n", conn->toString().c_str());
}


}// end of namespace sh
