 ///
 /// @file    thread.h
 /// @author  yangwenhao
 /// @date    2017-05-09 19:53:59
 ///
 
#ifndef __WD_THREAD_H__
#define __WD_THREAD_H__

#include "Noncopyable.h"

#include <pthread.h>
#include <functional>
#include <map>

namespace sh
{

class Thread : Noncopyable
{
public:
	typedef std::function<void()> ThreadCallback;

	Thread(ThreadCallback cb);
	~Thread();

	void start();
	void join();
#if 0//或者通过类Noncopyable实现不可复制
	Thread(const Thread &) =delete;//=delete C++11的特性,等价于private
	Thread & operator=(const Thread &) =delete;//表达对象语义
#endif
private:
	//pthread_create需要传一个参数,故设置为static
	static void * threadFunc(void *);//类内使用,不是对外接口,设置为私有
private:
	pthread_t _pthID;
	bool _isRunning;
	
	ThreadCallback _cb;
};


}//end of namespace sh

#endif
