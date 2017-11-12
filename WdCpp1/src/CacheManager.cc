 ///
 /// @file    CacheManager.cc
 /// @author  yangwenhao
 /// @date    2017-05-21 22:53:35
 ///
 
#include "CacheManager.h"
#include "Configuration.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;


namespace sh
{

CacheManager::CacheManager(int initTime, int intervalTime)
: _conf(Configuration::getConfigurationPtr())
, _timerThread(initTime, intervalTime, std::bind(&CacheManager::periodicUpdata, this))
{
}

void CacheManager::init(CachePtrMap cachePtrMap)
{
	_cachePtrMap = cachePtrMap;										//必须等所有线程创建完毕,才能获得各个线程的cachePtr

	bool hasLocalCahce;								
	hasLocalCahce = getCachePtr(0)->readFromFile();					//从本地cache文件读入信息
	if(hasLocalCahce && (!getCachePtr(0)->getCacheMap().empty()))		//如果有cache文件且已将文件数据读入主线程缓存中
	{
		mainCacheUpdataThreadCache();								//将主线程cache写入到各个子线程的cache中
	}

	_timerThread.start();											//启动timer定时器
	sleep(1);														//主线程等待定时器中子线程的创建
}

void CacheManager::periodicUpdata()
{
	for(int idx = 1; idx != _cachePtrMap.size(); ++idx)				//更新所有子线程的cache
	{
		if(!getCachePtr(idx)->getCacheMap().empty())					//如果该子线程cache有数据
		{
			getCachePtr(0)->getCacheMap().insert(getCachePtr(idx)->getCacheMap().begin(), 
											   getCachePtr(idx)->getCacheMap().end());//将子线程cache的数据写入到主线程的cache
			getCachePtr(idx)->getCacheMap().clear();		//将子线程cache的数据清空
		}
	}
	mainCacheUpdataThreadCache();	
	getCachePtr(0)->writeToFile();							//将主线程的cache写到磁盘cache文件中	
}

void CacheManager::mainCacheUpdataThreadCache()
{
	for(int idx = 1; idx != _cachePtrMap.size(); ++idx)
	{
		getCachePtr(idx)->getCacheMap().insert(getCachePtr(0)->getCacheMap().begin(),
											getCachePtr(0)->getCacheMap().end());//将主线程cache分配到各子线程
	}
}

CachePtr CacheManager::getCachePtr(int a)	
{  
	return _cachePtrMap[a];											//获取每个a对应的cache
}

}//end of namespace sh
