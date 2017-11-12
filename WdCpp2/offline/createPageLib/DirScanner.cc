 ///
 /// @file    DirScanner.cc
 /// @author  yangwenhao
 /// @date    2017-05-27 15:29:03
 ///
 
#include "DirScanner.h"
#include "Configuration.h"

#include <sys/types.h>
#include <dirent.h>
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

DirScanner::DirScanner()
{
}

void DirScanner::operator()(const string & dirname)
{
	traverse(Configuration::getConfigPtr()->getPath(dirname));
}

void DirScanner::traverse(const string & dirname)
{
	DIR * dir = opendir(dirname.c_str());
	if(NULL == dir)
	{
		cout << "DirScanner: opendir error!" << endl;
		exit(EXIT_FAILURE);
	}
	struct dirent * p;
	while(NULL != (p = readdir(dir)))
	{
		string path = dirname;
		if(p->d_name[0] != '.')
		{
			path.append("/").append(p->d_name);
			_vecFiles.push_back(path);
		}
	}
}

void DirScanner::showVecFiles()
{
	for(auto & elem : _vecFiles)
	{
		cout << elem << endl;
	}
}

}// end of namespace sh
