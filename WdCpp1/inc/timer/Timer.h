 ///
 //cout << "success" << endl;
 /// @file    Timer.h
 /// @author  yangwenhao
 /// @date    2017-05-15 09:5:48
 ///
 
#ifndef __TIMER_H__
#define __TIMER_H__

#include "Noncopyable.h"
#include <functional>

namespace sh
{

class Timer : Noncopyable
{
public:
	typedef std::function<void()> TimerCallback;

	Timer(int initTime, int intervalTime, TimerCallback cb);
	~Timer();

	void start();
	void stop();

private:
	int createTimerfd();
	void setTimerfd(int initTime, int intervalTime);
	void handleRead();

private:
	int _fd;
	int _initTime;
	int _intervalTime;
	bool _isStarted;
	TimerCallback _cb;
};

}// end of namespace sh

#endif
