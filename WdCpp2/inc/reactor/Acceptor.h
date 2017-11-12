 ///
 /// @file    acceptor.h
 /// @author  yangwenhao
 /// @date    2017-05-12 19:52:19
 ///
 
#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "Socket.h"
#include "InetAddress.h"

namespace sh
{

class Acceptor
{
public:
	Acceptor(int listenfd, const InetAddress & addr);
	
	void ready();
	int accept();
	int fd() const;
private:
	void setReuseAddr(bool on);
	void setReusePort(bool on);
	void bind();
	void listen();
private:
	Socket _listenSock;
	InetAddress _addr;
};

}// end of namespace sh


#endif
