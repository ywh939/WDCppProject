 ///
 /// @file    Dictionary.cc
 /// @author  yangwenhao
 /// @date    2017-05-18 16:14:02
 ///
 
#include "Dictionary.h"
#include "Configuration.h"
#include "TaskUtil.h"

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

Dictionary * Dictionary::_pDict = getDictPtr();
Dictionary::AutoRelease Dictionary::_autoRelease;

Dictionary::AutoRelease::~AutoRelease()
{
	if(_pDict)
		delete _pDict;
}

Dictionary * Dictionary::getDictPtr()
{
	if(NULL == _pDict)
	{
		_pDict = new Dictionary;
	}
	_pDict->init();
	return _pDict;
}

Dictionary::Dictionary()
: _conf(Configuration::getConfigurationPtr())
{
}

void Dictionary::init()
{
	initDict("cn_dict", true);
	initDict("en_dict", false);
	initIndex(_cnDict);
	initIndex(_dict);
}

void Dictionary::showIndexTable()
{
	ofstream ofs("./IndexTable.dat");
	if(!ofs.good())
	{
		cout << "Dictionary: ofs IndexTable.dat error!" << endl;
		exit(EXIT_FAILURE);
	}
	for(auto & mapElem : _index)
	{
		ofs << mapElem.first << " : ";
		for(auto & setElem : mapElem.second)
		{
			ofs << setElem << " ";
		}
		ofs << endl;
	}
}

void Dictionary::initDict(const string dictName, bool isCN)
{
	ifstream ifs(_conf->query(dictName));
	if(!ifs.good())
	{
		cout << "Dictionary: ifstream dict error" << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	string word;
	int freq;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		iss >> word >> freq;
		if(isCN)
			_cnDict.push_back(std::make_pair(word, freq));
		else
			_dict.push_back(std::make_pair(word, freq));
	}
	ifs.close();
}

void Dictionary::initIndex(VectorDict & vec)
{
	for(size_t i = 0; i != vec.size(); ++i)
	{
		int pos = 0;
		for(size_t j = 0; j != vec[i].first.size(); ++j)
		{
			int nBytes = nBytesCode(vec[i].first[j]);			//定义在TaskUtil.h
			string word = vec[i].first.substr(pos, nBytes);
			_index[word].insert(i);

			j += (nBytes - 1);
			pos += nBytes;
		}
	}
//	showIndexTable();
}

}// end of namespace sh
