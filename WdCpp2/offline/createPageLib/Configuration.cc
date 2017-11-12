 ///
 /// @file    Configuration.cc
 /// @author  yangwenhao
 /// @date    2017-05-26 20:53:33
 ///

#include "Configuration.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;


namespace sh
{

Configuration * Configuration::_configPtr = Configuration::getConfigPtr();

Configuration * Configuration::getConfigPtr()
{
	if(NULL == _configPtr)
	{
		_configPtr = new Configuration;
		atexit(destroy);
	}
	return _configPtr;
}

void Configuration::destroy()
{
	if(_configPtr)
		delete _configPtr;
}

void Configuration::init(const string & filepath)
{
	ifstream ifs(filepath);
	if(!ifs.good())
	{
		cout << "Configuration: ifs error!" << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		string key, value;
		iss >> key >> value;
		_configMap[key] = value;
	}
	ifs.close();
}

void Configuration::show()
{
	for(auto & elem : _configMap)
	{
		cout << elem.first << "-->" << elem.second << endl;
	}
}

}// end of namespace sh
