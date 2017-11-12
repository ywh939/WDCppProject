 ///
 /// @file    thread.cc
 /// @author  yangwenhao
 /// @date    2017-05-09 20:10:04
 ///
 
#include "Thread.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

Thread::Thread(ThreadCallback cb)
: _pthID(0)
, _isRunning(false)
, _cb(cb)
{}

Thread::~Thread()
{
	if(_isRunning)
		pthread_detach(_pthID);//将线程运行权交给系统,由系统进行托管
	_isRunning = false;
}

void Thread::start()
{
	pthread_create(&_pthID, NULL, threadFunc, this);
	_isRunning = true;
}

void Thread::join()
{
	pthread_join(_pthID, NULL);
	_isRunning = false;
}

void * Thread::threadFunc(void * arg)
{
	Thread * p = static_cast<Thread *>(arg);
	if(p)
		p->_cb();
	return NULL;
}


};//end of namespce sh
