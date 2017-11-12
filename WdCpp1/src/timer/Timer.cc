 ///
 /// @file    Timer.cc
 /// @author  yangwenhao
 /// @date    2017-05-15 10:13:38
 ///
 
#include "Timer.h"
#include <poll.h>
#include <sys/timerfd.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

Timer::Timer(int initTime, int intervalTime, TimerCallback cb)
: _fd(createTimerfd())
, _initTime(initTime)
, _intervalTime(intervalTime)
, _isStarted(false)
, _cb(cb)
{}

Timer::~Timer()
{
	cout << "~Timer()" << endl;
//	if(_isStarted)
//		stop();
}

void Timer::start()
{
	_isStarted = true;
	setTimerfd(_initTime, _intervalTime);

	struct pollfd pfd;
	pfd.fd = _fd;
	pfd.events = POLLIN;

	while(_isStarted)
	{
		int nready = poll(&pfd, 1, 5000);
		if(-1 == nready)
		{
			if(errno == EINTR)
				continue;
			perror("poll error");
			return;
		}
		else if(0 == nready)
		{
			cout << "> poll timeout!" << endl;
		}
		else
		{
			if(pfd.fd == _fd && (pfd.revents & POLLIN))
			{
				handleRead();//对定时器做超时处理
				if(_cb)
					_cb();
			}
		}
	}
}

void Timer::stop()
{
	if(_isStarted)
	{
		setTimerfd(0, 0);
		_isStarted = false;
		cout << "Timer::stop()" << endl;
		return;
	}
}

int Timer::createTimerfd()
{
	int fd = timerfd_create(CLOCK_REALTIME, 0);
	if(-1 == fd)
	{
		perror("timerfd_create error");
	}
	return fd;
}

void Timer::setTimerfd(int initTime, int intervalTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = intervalTime;
	value.it_interval.tv_nsec = 0;

	int ret = timerfd_settime(_fd, 0, &value, NULL);
	if(-1 == ret)
	{
		perror("timerfd_settime error");
	}
}

void Timer::handleRead()
{
	uint64_t howmany = 1;
	int ret = read(_fd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany))
	{
		perror("read error");
	}
}

}// end of namespace sh
