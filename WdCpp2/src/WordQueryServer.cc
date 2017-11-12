 ///
 /// @file    WordQueryServer.cc
 /// @author  yangwenhao
 /// @date    2017-06-02 20:43:44
 ///
 
#include "WordQueryServer.h" 
#include "GlobalDefine.h"
#include "TaskUtil.h"
#include "Task.h"

#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using std::placeholders::_1;

namespace sh
{

WordQueryServer::WordQueryServer()
: _conf(Configuration::getConfigPtr())
, _wordQuery(WordQuery())
, _threadpool(string2size_t(_conf->getValue(BUFF_SIZE)), string2size_t(_conf->getValue(THREAD_NUM)))
, _tcpServer(_conf->getValue(IP), string2unShort(_conf->getValue(PORT)))
{
}

void WordQueryServer::start()
{
	_tcpServer.setConnectionCallback(bind(&WordQueryServer::onConnection, this, _1));
	_tcpServer.setMessageCallback(bind(&WordQueryServer::onMessage, this, _1));
	_tcpServer.setCloseCallback(bind(&WordQueryServer::onClose, this, _1));

	_threadpool.start();											//启动线程池
	sleep(1);
	_tcpServer.start();												//启动epoll_wait
}

void WordQueryServer::onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << endl;								//将连接的客户端ip和端口号打印出来
//	conn->send("hello, welcome to Chat Server.\r\n");
}

void WordQueryServer::onMessage(const TcpConnectionPtr & conn)
{
	string msg(conn->receive());					//调用conn的recevie()用来将服务器接收到的数据从内核态读到用户态
	doTaskThread(conn, msg);
}

void WordQueryServer::onClose(const TcpConnectionPtr & conn)
{
	printf("%s close\n", conn->toString().c_str());
}

void WordQueryServer::doTaskThread(const TcpConnectionPtr & conn, const string & msg)
{
	TaskPtr task(new Task(msg, conn, _wordQuery));	//创建一个任务来解决msg
	_threadpool.addTask(task);					//将任务添加到buffer中
}

}// end of namespace sh
