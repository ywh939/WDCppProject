 ///
 /// @file    buffer.cc
 /// @author  yangwenhao
 /// @date    2017-05-10 10:54:36
 ///
 
#include "Buffer.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

Buffer::Buffer(size_t sz)
: _mutex()					//mutexLock无参构造函数,故写成()
, _notFull(_mutex)			//初始化两个条件变量
, _notEmpty(_mutex)
, _sz(sz)					//buffer
, _flag(true)
{}

bool Buffer::full()
{
	return _sz == _que.size();
}

bool Buffer::empty()
{
	return _que.empty();
}

void Buffer::push(const ElemType & elem)
{
	MutexGuard guard(_mutex);
	while(full())
		_notFull.wait();

	_que.push(elem);				//将任务添加到队列中,并通知buffer队列不空,可以从队列拿到任务
	_notEmpty.notify();
}

ElemType Buffer::pop()
{
	MutexGuard guard(_mutex);		//线程共享buffer,故要设置条件变量
	while(_flag && empty())			//flag初始设置为true,为了避免程序内主动停止运行时各线程阻塞在while(empty())
		_notEmpty.wait();			//即主动停止时调用wakeup()将flag设置为false,直接return NULL
	if(_flag)
	{
		ElemType tmp = _que.front();//获取队头元素,taskPtr
		_que.pop();					//获取后删除
		_notFull.notify();			//通知buffer队列不满,可以往buffer的队列添加task任务
		return tmp;					
	}else
		return NULL;
}

void Buffer::wakeup()
{
	_flag = false;
	_notEmpty.notifyAll();			//通知所有线程
}


}//end of namespace sh
