 ///
 /// @file    Configuration.h
 /// @author  yangwenhao
 /// @date    2017-05-26 20:47:17
 ///
 
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include <map>
#include <set>
using std::string;
using std::map;
using std::set;

namespace sh
{

class Configuration
{
public:
	static Configuration * getConfigPtr();
	static void destroy();
	void init(const string & filepath);

	void show();

	string & getValue(const string & key) {	return _configMap[key];	}
	set<string> getStopWordList()	{	return _stopWordList;	}

private:
	Configuration() {}
	~Configuration() {}

private:
	static Configuration * _configPtr;
	map<string, string> _configMap;	//配置文件内容
	set<string> _stopWordList;		//停用词词集
};

}// end of namespace sh

#endif
