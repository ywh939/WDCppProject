 ///
 /// @file    Configuration.cc
 /// @author  yangwenhao
 /// @date    2017-05-16 17:11:29
 ///
 
#include "Configuration.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace sh
{
Configuration * Configuration::_confPtr = Configuration::getConfigurationPtr();

Configuration * Configuration::getConfigurationPtr()
{
	if(NULL == _confPtr)
	{
		_confPtr = new Configuration("/home/lenovo/project/offline/conf/Configuration.conf");
		atexit(destroy);
	}
	return _confPtr;
}

void Configuration::destroy()
{
	if(_confPtr)
		delete _confPtr;
}

Configuration::Configuration(const string & confpath)
: _confpath(confpath)
{
	handlefile();
}

const string & Configuration::query(const string & filename)
{
	auto it = _confMap.find(filename);
	if(it == _confMap.end())
	{
		cout << "Configuration error" << endl;
		exit(EXIT_FAILURE);
	}
	return it->second;
}

void Configuration::handlefile()
{
	ifstream ifs(_confpath);
	if(!ifs.good())
	{
		cout << "ifstream _confpath error" << endl;
		return;
	}

	string line;
	string filename;
	string filepath;

	while(getline(ifs, line))
	{
		istringstream iss(line);
		iss >> filename >> filepath;
		_confMap.insert(std::make_pair(filename, filepath));
	}
	ifs.close();
}

}// end of namespace sh
