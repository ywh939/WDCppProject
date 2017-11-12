 ///
 /// @file    SpellcorrectServer.h
 /// @author  yangwenhao
 /// @date    2017-05-17 19:37:27
 ///
 
#ifndef __SPELLCORRCETSERVER_H__
#define __SPELLCORRCETSERVER_H__

#include "Configuration.h"
#include "Threadpool.h"
#include "TcpServer.h"
#include "CacheManager.h"

namespace sh
{

class SpellcorrectServer
{
public:
	SpellcorrectServer(
			size_t buffsize, 
			size_t threadNum, 
			int initTime, 
			int intervalTime, 
			int wordNum);

	void start();

private:
	void onConnection(const TcpConnectionPtr & conn);
	void onMessage(const TcpConnectionPtr & conn);
	void onClose(const TcpConnectionPtr & conn);

private:
	Configuration * _conf;
	size_t			_buffsize;
	size_t			_threadNum;
	int				_wordNum;
	Threadpool		_threadpool;
	TcpServer		_tcpserver;
	CacheManager	_cacheManager;
};

}// end of namespace sh

#endif
