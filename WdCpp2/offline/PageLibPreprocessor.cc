 ///
 /// @file    PageLibPreprocessor.cc
 /// @author  yangwenhao
 /// @date    2017-05-29 11:47:26
 ///
 
#include "PageLibPreprocessor.h"
#include "TaskUtil.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;


namespace sh
{

PageLibPreprocessor::PageLibPreprocessor()
: _conf(Configuration::getConfigPtr())
, _jieba(CppJieba())
{
}

void PageLibPreprocessor::doProcess()
{
	cout << "readInfoFromFile()" << endl;
	readInfoFromFile();
	cout << "cutRedundantPages()" << endl;
	cutRedundantPages();
	cout << "builtInvertIndexTable()" << endl;
	builtInvertIndexTable();
	storeOnDisk();
}

void PageLibPreprocessor::readInfoFromFile()
{
	ifstream ifsOffset(_conf->getPath("newoffset"));
	if(!ifsOffset.good())
	{
		cout << "PageLibPreprocessor: ifs newoffset error!" << endl;
		::exit(EXIT_FAILURE);
	}
	ifstream ifsPage(_conf->getPath("newfipepage"));
	if(!ifsPage.good())
	{
		cout << "PageLibPreprocessor: ifs newfipepage error!" << endl;
		::exit(EXIT_FAILURE);
	}
	string line;
	while(getline(ifsOffset, line))
	{
		istringstream iss(line);
		string word;
		int idx = 0;
		int docid, offset, length;
		while(iss >> word)
		{
			if(0 == idx)
				docid = string2int(word);
			if(2 == idx)
				offset = string2int(word);
			if(3 == idx)
				length = string2int(word);
			++idx;
		}

		ifsPage.seekg(offset, ifsPage.beg);
		char buf[65536] = {0};
		ifsPage.read(buf, length);
		string doc = convert2string(buf);

		WebPage webPage(doc, _jieba);
		_pageLib.push_back(webPage);
	}
	ifsPage.close();
	ifsOffset.close();
}

void PageLibPreprocessor::cutRedundantPages()
{
	set<int, std::greater<int> > redundantPos;//这里设置greater,以便于从尾部进行删除
	for(size_t i = 0; i != _pageLib.size(); ++i)
	{
		for(size_t j = i + 1; j != _pageLib.size(); ++j)
		{
			if(_pageLib[i].getWordsMap().size() >= 10 &&
			   _pageLib[j].getWordsMap().size() >= 10)
			{
				if(_pageLib[i] == _pageLib[j])
				{
					if(_pageLib[i] < _pageLib[j])
					{
						redundantPos.insert(j);
					}
					else
					{
						redundantPos.insert(i);
					}
				}
			}// end of if()
		}// end of for()
	}// end of for()
	for(auto & elem : redundantPos)
	{
	//	cout << elem << " ";
		_pageLib.erase(_pageLib.begin() + elem);
	}
	//cout << endl;
}

void PageLibPreprocessor::builtInvertIndexTable()
{
	map<string, int> docFreqMap;	//某个词在所有文章中出现的次数
	for(size_t i = 0; i != _pageLib.size(); ++i)
	{
		for(auto & elem : _pageLib[i].getWordsMap())
		{
			string word = elem.first;
			auto it = docFreqMap.find(word);
			if(it == docFreqMap.end())			//和前面相同的词语不再计算
			{
				int DocFreq = 1;		//代表当前文章出现的词语,记为1次
				for(size_t j = i + 1; j != _pageLib.size(); ++j)
				{
					auto it = _pageLib[j].getWordsMap().find(word);
					if(it != _pageLib[j].getWordsMap().end())
					{
						++DocFreq;
					}
				}
				docFreqMap[word] = DocFreq;
			}
		}// end of for()
	}// end of for()

#if 0
	for(auto & elem : docFreqMap)
	{
		cout << elem.first << " " << elem.second << endl;
	}
#endif

	double N = (double)_pageLib.size();
	for(size_t i = 0; i != _pageLib.size(); ++i)
	{
		map<string, pair<int, double> > termTable;	//每篇文档未归一化处理的权重
		int docid = _pageLib[i].getDocId();
		double sum = 0;
		for(auto & elem : _pageLib[i].getWordsMap())
		{
			string word = elem.first;

			double termFreq = (double)elem.second;
			double docFreq = (double)docFreqMap.find(word)->second;//int要转换为double,否则会发生inf
			double inverseDocFreq = ::log2(N/(docFreq + 1)); 

			double w = termFreq * inverseDocFreq;

			termTable[word] = std::make_pair(docid, w);

			double product = w * w;
			sum += product;
			if(sum > std::numeric_limits<double>::max())//判断超出类型的范围
			{
				cout << "PageLibPreprocessor: sum is inf" << endl;
				exit(EXIT_FAILURE);
			}
		}
		//对一篇文档中的每个词进行归一化处理
		for(auto & elem : termTable)
		{
			double w = elem.second.second;
			double W = w/sqrt(sum);
			_invertIndexTable[elem.first].insert(
					std::make_pair(elem.second.first, W));
		}
	}// end of for()
}

void PageLibPreprocessor::storeOnDisk()
{
	if(!_pageLib.empty())
	{
		ofstream ofs(_conf->getPath("fipepage"));
		if(!ofs.good())
		{
			cout << "PageLibPreprocessor: ofs fipepage error!" << endl;
			::exit(EXIT_FAILURE);
		}

		int pos = ofs.tellp();
		for(auto & elem : _pageLib)
		{
			ofs << elem.getDoc() << endl;
			int poscur = ofs.tellp();
			int length = poscur - pos;

			_offsetLib[elem.getDocId()] = std::make_pair(pos, length);
			pos = poscur;
		}
		ofs.close();
	}
	if(!_offsetLib.empty())
	{
		ofstream ofs(_conf->getPath("offset"));
		if(!ofs.good())
		{
			cout << "PageLibPreprocessor: ofs fipepage error!" << endl;
			::exit(EXIT_FAILURE);
		}

		for(auto & elem : _offsetLib)
		{
			ofs << elem.first << " : "
				<< elem.second.first << " "
				<< elem.second.second << endl;
		}
		ofs.close();
	}
	if(!_invertIndexTable.empty())
	{
		ofstream ofs(_conf->getPath("invertIndex"));
		if(!ofs.good())
		{
			cout << "PageLibPreprocessor: ofs invertIndex error!" << endl;
			::exit(EXIT_FAILURE);
		}
		for(auto & elem : _invertIndexTable)
		{
			ofs << elem.first << " : ";
			for(auto & setElem : elem.second)
			{
				ofs << "( " << setElem.first << " " 
					<< setElem.second << " ) ";
			}
			ofs << endl;
		}
		ofs.close();
	}
}

}// end of namespace sh
