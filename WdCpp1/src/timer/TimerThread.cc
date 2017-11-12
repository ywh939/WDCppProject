 ///
 /// @file    TimerThread.cc
 /// @author  yangwenhao
 /// @date    2017-05-15 10:11:16
 ///
 
#include "TimerThread.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

TimerThread::TimerThread(int initTime, int intervalTime, TimerCallback cb)
: _timer(initTime, intervalTime, cb)
, _thread(std::bind(&Timer::start, &_timer))
{
}

TimerThread::~TimerThread()
{
}

void TimerThread::start()
{
	_thread.start();
}

#if 0
void TimerThread::stop()
{
	cout << "TimerThread::stop()" << endl;
	_timer.stop();
	_thread.join();
}
#endif

}// end of namespace sh
