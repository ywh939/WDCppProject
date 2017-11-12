 ///
 /// @file    TimerThread.h
 /// @author  yangwenhao
 /// @date    2017-05-15 10:05:13
 ///
 
#include "Timer.h"
#include "Thread.h"
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

class TimerThread
{
public:
	typedef std::function<void()> TimerCallback;

	TimerThread(int initTime, int intervalTime, TimerCallback cb);
	~TimerThread();

	void start();
	void stop();

private:
	Timer _timer;
	Thread _thread;
};

}// end of namespace sh
