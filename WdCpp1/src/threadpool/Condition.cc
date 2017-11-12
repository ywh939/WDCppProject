 ///
 /// @file    condition.cc
 /// @author  yangwenhao
 /// @date    2017-05-10 20:32:28
 ///
 
#include "Condition.h"
#include "MutexLock.h"

namespace sh
{

Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{
	pthread_cond_init(&_cond, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
	pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
}

void Condition::notify()
{
	pthread_cond_signal(&_cond);
}

void Condition::notifyAll()
{
	pthread_cond_broadcast(&_cond);
}

}// end of namespace sh
