 ///
 /// @file    MutexLock.h
 /// @author  yangwenhao
 /// @date    2017-05-10 10:29:04
 ///
 
#ifndef __WD_MUTEXLOCK_H__
#define __WD_MUTEXLOCK_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace sh
{

class MutexLock : Noncopyable
{
public:
	MutexLock();
	~MutexLock();

	pthread_mutex_t * getMutexLockPtr();

	void lock();
	void unlock();
private:
	pthread_mutex_t _mutex;
};

class MutexGuard
{
public:
	MutexGuard(MutexLock & mutex);
	~MutexGuard();
private:
	MutexLock & _mutex;
};

}

#endif
