 ///
 /// @file    CommonServer.cc
 /// @author  yangwenhao
 /// @date    2017-06-02 21:07:40
 ///
 
#include "TaskUtil.h"
#include "GlobalDefine.h"
#include "CommonServer.h"
#include "Task.h"

#include <unistd.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using std::placeholders::_1;

namespace sh
{

CommonServer::CommonServer()
: _conf(Configuration::getConfigPtr())
, _threadpool(string2size_t(_conf->getValue(BUFF_SIZE)), string2size_t(_conf->getValue(THREAD_NUM)))
, _tcpServer(_conf->getValue(IP), string2unShort(_conf->getValue(PORT)))
{
}

void CommonServer::start()
{
	_tcpServer.setConnectionCallback(bind(&CommonServer::onConnection, this, _1));
	_tcpServer.setMessageCallback(bind(&CommonServer::onMessage, this, _1));
	_tcpServer.setCloseCallback(bind(&CommonServer::onClose, this, _1));//在EpollPoller要传参,故用占用符

	_threadpool.start();											//启动线程池
	sleep(1);														//等待线程池启动完毕
	_tcpServer.start();												//启动epoll_wait
}

void CommonServer::onConnection(const TcpConnectionPtr & conn)
{
	cout << conn->toString() << endl;								//将连接的客户端ip和端口号打印出来
	conn->send("hello, welcome to Chat Server.\r\n");
}

void CommonServer::onMessage(const TcpConnectionPtr & conn)
{
	string msg(conn->receive());					//调用conn的recevie()用来将服务器接收到的数据从内核态读到用户态
	doTaskThread(conn, msg);
}

void CommonServer::onClose(const TcpConnectionPtr & conn)
{
	printf("%s close\n", conn->toString().c_str());
}

void CommonServer::doTaskThread(const TcpConnectionPtr & conn, const string & msg)
{
	TaskPtr task(new Task(msg, conn));			//创建一个任务来处理msg
	_threadpool.addTask(task);					//将任务添加到buffer中
}

}// end of namespace
