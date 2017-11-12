 ///
 /// @file    condition.h
 /// @author  yangwenhao
 /// @date    2017-05-10 20:29:13
 ///
 
#ifndef __WD_CONDITION_H__
#define __WD_CONDITION_H__

#include "Noncopyable.h"
#include <pthread.h>

namespace sh
{

class MutexLock;

class Condition : Noncopyable
{
public:
	Condition(MutexLock & mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();
private:
	MutexLock & _mutex;
	pthread_cond_t _cond;
};

}// end of namespace sh

#endif
