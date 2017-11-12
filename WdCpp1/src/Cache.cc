 ///
 /// @file    Cache.cc
 /// @author  yangwenhao
 /// @date    2017-05-21 16:23:25
 ///
 
#include "Cache.h"
#include "Configuration.h"
#include "TaskUtil.h"

#include <stdlib.h>
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

Cache::Cache()
: _conf(Configuration::getConfigurationPtr())
, _isQueryWord(true)
{
}

void Cache::addElement(const string & queryWord, const string & msg)
{
	_cacheMap[queryWord] = msg;							//将queryWord-->msg写入map(即缓存)
}

void Cache::writeToFile()
{
	cout << "-------Cache::writeToCacheFile()-------" << endl;
	ofstream ofs(_conf->query("cache"));
	if(!ofs.good())
	{
		cout << "ofstream cache.dat error!" << endl;
		exit(EXIT_FAILURE);
	}

	for(auto & elem : _cacheMap)
	{
		ofs << elem.first << " : " << elem.second << endl;
	}
	ofs.close();
}

bool Cache::readFromFile()
{
//	cout << "-------Cache::readFromFile()-------" << endl;
	ifstream ifs(_conf->query("cache"));
	if(!ifs.good())
	{
	//	cout << "-----ofstream------" << endl;
		ofstream ofs(_conf->query("cache"));				//如果路径下没有cache文件,则创建一个
		if(!ofs.good())
		{
			cout << "ofstream cache error!" << endl;
			exit(EXIT_FAILURE);
		}
		ofs.close();
		return false;
	}
	else
	{
		string line;
		string word;
		string queryWord;
		string msg;
		while(getline(ifs, line))
		{
			istringstream iss(line);	
			while(iss >> word)					
			{
				if(_isQueryWord)				//先设置为true,一般情况一行中第一个即为queryWord
				{
					queryWord = word;			
					_isQueryWord = false;		//设置false,避免后面的word读入
				}
				else if(word != ":")
				{
					msg.append(word);			//将 : 后的word写在msg中
					msg.append(" ");
				}
			}
			_cacheMap[queryWord] = msg;			//将qureyWord-->msg 写入map(即缓存)
			_isQueryWord = true;				//再设置为true,因为要读下一行
			msg.clear();
			queryWord.clear();
		}//end of while()
	}// end of else()
	ifs.close();
	return true;
}

}// end of namespace sh
