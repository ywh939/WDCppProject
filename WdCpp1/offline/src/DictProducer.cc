 ///
 /// @file    DictProducer.cc
 /// @author  yangwenhao
 /// @date    2017-05-23 22:57:55
 ///
 
#include "DictProducer.h"
#include "Configuration.h"
#include "SplitTool.h"
#include "CppJieba.h"

#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
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

DictProducer::DictProducer()
: _conf(Configuration::getConfigurationPtr())
, _enYuliaoDir(_conf->query("en_yuliao"))
, _cnYuliaoDir(_conf->query("cn_yuliao"))
, _splitToolPtr(new CppJieba)
, _distCnChar()
, _stopWords()
{
}

void DictProducer::built_en_dict()
{
	builtFilePathVector(_enYuliaoDir);
	pushDict(false);
	storeDictToDisk(_conf->query("en_dict"));						//创建本地英文词典
	_vecFilePath.clear();
}

void DictProducer::built_cn_dict()
{
	builtFilePathVector(_cnYuliaoDir);
	pushDict(true);
	storeDictToDisk(_conf->query("cn_dict"));							//创建本地中文词典
	_vecFilePath.clear();
}

void DictProducer::builtFilePathVector(const string & str)
{
	DIR * dir;
	dir = opendir(str.c_str());
	if(NULL == dir)
	{
		cout << "DictProducer: opendir _enYuliaoDir error!" << endl;
		exit(EXIT_FAILURE);
	}
	struct dirent * p;
	while(NULL != (p = readdir(dir)))
	{
		string yuliaoDir = str;
		if(p->d_name[0] != '.')
		{
			yuliaoDir.append("/").append(p->d_name);//路径拼接
			_vecFilePath.push_back(yuliaoDir);
		}
	}
}

void DictProducer::pushDict(bool isCnDict)
{
	for(auto & elem : _vecFilePath)
	{
		ifstream ifs(elem);
		if(!ifs.good())
		{
			cout << "DictProducer: ifs filepath error!" << endl;
			exit(EXIT_FAILURE);
		}

		string line;
		string word;
		while(getline(ifs, line))
		{
			if(isCnDict)
			{
				_vecWords = _splitToolPtr->cut(line);
				for(auto & elem : _vecWords)
				{
					if(_distCnChar.isCnCharacter(elem))
					{
						auto it = _stopWords.getStopWords(true).find(elem);
						if(it != _stopWords.getStopWords(true).end())
							continue;
					}
					else
					{
						continue;
					}
					++_mapDict[elem];
				}
				_vecWords.clear();
			}
			else
			{
				istringstream iss(line);
				while(iss >> word)
				{
					if(word[0] >= 48 && word[0] <= 57)
					{
						continue;
					}
					else 
					{
						auto it = _stopWords.getStopWords(false).find(word);//从英文停用词里找
						if(it != _stopWords.getStopWords(false).end())
							continue;
					}
					++_mapDict[word];
				}
			}
		}
		ifs.close();
	}// end of for()
}

void DictProducer::storeDictToDisk(const string dictPath)
{
	ofstream ofs(dictPath);
	if(!ofs.good())
	{
		cout << "DictProducer: ofs dictPath error!" << endl;
		exit(EXIT_FAILURE);
	}

	for(auto & elem : _mapDict)
	{
		ofs << elem.first << " " << elem.second << endl;
	}
	_mapDict.clear();
	ofs.close();
}

}// end of namespace sh
