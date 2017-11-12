 ///
 /// @file    CacheManager.h
 /// @author  yangwenhao
 /// @date    2017-05-21 22:43:23
 ///
 
#ifndef __CACHEMANAGER_H__
#define __CACHEMANAGER_H__

#include "Cache.h"
#include "TimerThread.h"

#include <vector>
#include <map>
#include <unordered_map>
using std::vector;
using std::map;
using std::unordered_map;

namespace sh
{

class Configuration;
class TimerThread;

class CacheManager
{
public:
	typedef map<int, CachePtr> CachePtrMap;

	CacheManager(int initTime, int intervalTime);
	void init(CachePtrMap cachePtrMap);
	void periodicUpdata();

private:
	CachePtr getCachePtr(int a);

	void mainCacheUpdataThreadCache();	

private:
	CachePtrMap _cachePtrMap;
	Configuration * _conf;
	TimerThread _timerThread;
	
};

}// end of namespace sh


#endif
