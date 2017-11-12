 ///
 /// @file    mutexLock.cc
 /// @author  yangwenhao
 /// @date    2017-05-10 20:23:30
 ///
 
#include "MutexLock.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

MutexLock::MutexLock()
{
	pthread_mutex_init(&_mutex, NULL);
}

MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&_mutex);
}

void MutexLock::lock()
{
	pthread_mutex_lock(&_mutex);
}

void MutexLock::unlock()
{
	pthread_mutex_unlock(&_mutex);
}

pthread_mutex_t * MutexLock::getMutexLockPtr()
{
	return &_mutex;
}

MutexGuard::MutexGuard(MutexLock & mutex)
: _mutex(mutex)
{
	_mutex.lock();
}

MutexGuard::~MutexGuard()
{
	_mutex.unlock();
}

}// end of namesapce sh
