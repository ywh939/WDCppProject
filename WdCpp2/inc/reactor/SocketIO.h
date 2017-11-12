 ///
 /// @file    socketIO.h
 /// @author  yangwenhao
 /// @date    2017-05-12 19:58:49
 ///
 
#ifndef __SOCKETIO_H__	
#define __SOCKETIO_H__	

#include <stdio.h>//size_t 的头文件

namespace sh
{

class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char * buf, size_t count);
	size_t writen(const char * buf, size_t count);	
	size_t readline(char * buf, size_t max_len);
private:
	size_t recv_peek(char * buf, size_t count);
private:
	int _sockfd;
};

}// end of namespace sh


#endif
