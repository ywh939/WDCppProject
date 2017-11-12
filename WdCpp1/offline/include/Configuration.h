 ///
 /// @file    Configurationiguration.cc
 /// @author  yangwenhao
 /// @date    2017-05-16 17:05:48
 ///
 
#ifndef __Configuration_H__
#define __Configuration_H__

#include "Noncopyable.h"
#include <map>
#include <string>
using std::map;
using std::string;

namespace sh
{

class Configuration : Noncopyable
{
public:
	static Configuration * getConfigurationPtr();
	static void destroy();
	const string & query(const string & filename);

private:
	Configuration(const string & confpath);
	void handlefile();

private:
	static Configuration * _confPtr;
	map<string, string> _confMap;
	string _confpath;
};

}// end of namespace sh

#endif
