 ///
 /// @file    Index.cc
 /// @author  yangwenhao
 /// @date    2017-05-17 10:12:35
 ///
 
#include "Index.h"
#include "Configuration.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;


namespace sh
{

Index::Index()
: _conf(Configuration::getConfigurationPtr())
{
}

void Index::createIndex()
{
	createCharSet();
	createIndexMap();
	writeIndexFile();
}

void Index::createCharSet()
{
	for(char ch = 'a'; ch <= 'z'; ++ch)
	{
		_charSet.insert(ch);
	}
}

void Index::createIndexMap()
{
	ifstream ifs(_conf->query("en_dict"));
	if(!ifs.good())
	{
		cout << "ifstream dict error!" << endl;
		exit(EXIT_FAILURE);
	}
	
	string stringLine;
	string word;
	int freq;
	int intLine = 0;
	while(getline(ifs, stringLine))
	{
		istringstream iss(stringLine);
		iss >> word >> freq;

		for(size_t idx = 0; idx != word.size(); ++idx)
		{
			auto it = _charSet.find(word[idx]);
			if(it != _charSet.end())
			{
				_indexMap[char2string(word[idx])].insert(intLine);
			}
		}
		++intLine;
	}
	ifs.close();
}

void Index::writeIndexFile() const
{
	ofstream ofs(_conf->query("en_index"));
	if(!ofs.good())
	{
		cout << "ofstream indx.dat error!" << endl;
		exit(EXIT_FAILURE);
	}

	for(auto & elem : _indexMap)
	{
		ofs << elem.first << " : ";
		for(auto & integer : elem.second)
		{
			ofs << integer << " ";
		}
		ofs << endl;
	}
	ofs.close();
}

string Index::char2string(char ch)//返回值不能是引用,s是局部变量
{
	ostringstream oss;
	oss << ch;
	return oss.str();
}

}// end of namespace sh
