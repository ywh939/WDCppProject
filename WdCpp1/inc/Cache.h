 ///
 /// @file    Cache.h
 /// @author  yangwenhao
 /// @date    2017-05-21 14:11:51
 ///
 
#ifndef __CACHE_H__
#define __CACHE_H__

#include <string>
#include <unordered_map>
#include <memory>
using std::string;
using std::unordered_map;
using std::shared_ptr;

namespace sh
{

class Configuration;
class Cache;

typedef shared_ptr<Cache> CachePtr;
typedef unordered_map<string, string> CacheMap;

class Cache
{
public:
	Cache();

	void addElement(const string & queryWord, const string & msg);
	void writeToFile();
	bool readFromFile();

	CacheMap & getCacheMap() {	return _cacheMap;	}

private:
	CacheMap _cacheMap;

	Configuration * _conf;

	bool _isQueryWord;
};

}// end of namespace sh

#endif
