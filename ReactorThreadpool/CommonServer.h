 ///
 /// @file    CommonServer.h
 /// @author  yangwenhao
 /// @date    2017-06-02 21:05:11
 ///
 
#ifndef __COMMCONSERVER_H__
#define __COMMCONSERVER_H__

#include "Configuration.h"
#include "Threadpool.h"
#include "TcpServer.h"

namespace sh
{

class CommonServer
{
public:
	CommonServer();

	void start();

private:
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

	void doTaskThread(const TcpConnectionPtr & conn, const string & msg);

private:
	Configuration * _conf;
	Threadpool _threadpool;
	TcpServer _tcpServer;
};

}// end of namespace sh

#endif
