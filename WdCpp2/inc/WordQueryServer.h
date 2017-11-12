 ///
 /// @file    WordQueryServer.h
 /// @author  yangwenhao
 /// @date    2017-05-31 15:13:32
 ///
 
#ifndef __WORDQUERYSERVER_H__
#define __WORDQUERYSERVER_H__

#include "Configuration.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "WordQuery.h"

namespace sh
{

class WordQueryServer
{
public:
	WordQueryServer();

	void start();

private:
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

	void doTaskThread(const TcpConnectionPtr & conn, const string & msg);

private:
	Configuration * _conf;
	WordQuery _wordQuery;
	Threadpool _threadpool;
	TcpServer _tcpServer;
};

}// end of namespace sh

#endif
