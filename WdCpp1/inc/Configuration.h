 ///
 /// @file    Configurationiguration.cc
 /// @author  yangwenhao
 /// @date    2017-05-16 17:05:48
 ///
 
#ifndef __Configuration_H__
#define __Configuration_H__

#include <map>
#include <string>
using std::map;
using std::string;

namespace sh
{

class Configuration
{
public:
	static Configuration * getConfigurationPtr();
	void init(const string & confPath);

	static void destroy();
	const string & query(const string & filename);

private:
	Configuration(const string & str) { init(str);	}
	~Configuration() {}

private:
	static Configuration * _confPtr;
	map<string, string> _confMap;
};

}// end of namespace sh

#endif
