 ///
 /// @file    buffer.h
 /// @author  yangwenhao
 /// @date    2017-05-11 10:06:25
 ///
 
#ifndef __WD_BUFFER_H__
#define __WD_BUFFER_H__

#include "MutexLock.h"
#include "Condition.h"
#include "Task.h"
#include <queue>
using std::queue;

namespace sh
{

typedef TaskPtr ElemType;

class Buffer
{
public:
	Buffer(size_t sz);
	
	bool full();
	bool empty();
	void push(const ElemType & elem);
	ElemType pop();
	void wakeup();
private:
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	size_t    _sz;
	queue<ElemType> _que;
	bool _flag;
};

}// end of namespace sh


#endif
