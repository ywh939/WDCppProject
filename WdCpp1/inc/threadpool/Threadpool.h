 ///
 /// @file    threadPool.h
 /// @author  yangwenhao
 /// @date    2017-05-11 10:00:38
 ///
 
#ifndef __WD_THREADPOOL_H__
#define __WD_THREADPOOL_H__

#include "Buffer.h"
#include "Task.h"

#include <vector>
#include <map>
using std::vector;
using std::map;


namespace sh
{

namespace current_thread
{
extern __thread int a;
}

class Thread;
class ThreadpoolThread;

typedef map<int, CachePtr> CachePtrMap;

class Threadpool
{
	friend ThreadpoolThread;
public:
	Threadpool(size_t buffsize, size_t threadNum);
	~Threadpool();
	
	void start();
	void stop();
	void addTask(TaskPtr task);
	CachePtrMap & getCachePtrMap() {  return _cachePtrMap; }

private:
	TaskPtr getTask();
	void threadFunc(int idx);

private:
	size_t _buffsize;
	size_t _threadNum;

	vector<shared_ptr<Thread> > _threads;

	CachePtrMap _cachePtrMap;

	Buffer _buffer;
	bool _isExit;
};

}// end of namespace sh

#endif
