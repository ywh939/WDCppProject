 ///
 /// @file    WordQuery.cc
 /// @author  yangwenhao
 /// @date    2017-05-31 10:34:32
 ///
 
#include "GlobalDefine.h"
#include "TaskUtil.h"
#include "WordQuery.h"
#include "MyLog.h"
#include "./json/json.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace sh
{

WordQuery::WordQuery()
: _conf(Configuration::getConfigPtr())
, _jieba(CppJieba())
{
	loadLibrary();
}

string WordQuery::doQuery(const string & str)
{
	vector<string> queryWords = _jieba.cut(str);
	queryWords.erase(queryWords.end() - 1);		//针对搜索引擎,最后一位为\n
	for(auto & elem : queryWords)
	{
		auto it = _invertIndexTable.find(elem);//只要有一个查询词不在索引表
		if(it == _invertIndexTable.end())
		{
			return returnNoAnswer();
		}
	}

	vector<double> queryWordsWeightVec = getQueryWordsWeightVector(queryWords);
	MapVectorPair resultMap;		//存储网页库中含有查询词的文档、权重集
	if(executeQuery(queryWords, resultMap))
	{
		double XSum = 0;
		for(auto & elem : queryWordsWeightVec)
		{
			double weightValue = elem;
			XSum += weightValue * weightValue;
		}
		double X = sqrt(XSum);

		map<double, int, std::greater<double> > cosMap;
		for(auto & mapElem : resultMap)
		{
			int docid = mapElem.first;
			double sum = 0;
			double product = 0;
			int idx = 0;
			for(auto & vecElem : mapElem.second)
			{
				double weightValue = vecElem;
				sum += weightValue * weightValue;
				product += queryWordsWeightVec[idx] * weightValue;
				++idx;
			}

			double cos = (product) / (X * sum);
			cosMap[cos] = docid;
		}

		vector<int> docIdVec;
		for(auto & elem : cosMap)
		{
			docIdVec.push_back(elem.second);
		}
		string msg = createJson(docIdVec, queryWords);
		return msg;
	}// end of if()
	else
	{
		return returnNoAnswer();
	}
}

void WordQuery::loadLibrary()
{
	ifstream ifsOffset(_conf->getValue(OFFSETLIB_KEY));
	if(!ifsOffset.good())
	{
		LogError("file open faile!");
		exit(EXIT_FAILURE);
	}
	ifstream ifsPage(_conf->getValue(FIPEPAGELIB_KEY));
	if(!ifsPage.good())
	{
		cout << "WordQuery: ifs fipepagelib error!" << endl;
		exit(EXIT_FAILURE);
	}
	ifstream ifsInvert(_conf->getValue(INVERTINDEX_KEY));
	if(!ifsInvert.good())
	{
		cout << "WordQuery: ifs invertIndex error!" << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	while(getline(ifsOffset, line))
	{
		istringstream iss(line);
		string word;
		vector<string> vecStr;
		while(iss >> word)
		{
			vecStr.push_back(word);
		}

		int docid = string2int(vecStr[0]);
		int offset = string2int(vecStr[2]);
		int length = string2int(vecStr[3]);
		
#if 0
		cout << docid << " " << offset << " " << length << endl;
#endif

		_offsetLib[docid] = std::make_pair(offset, length);

		ifsPage.seekg(offset, ifsPage.beg);
		char buf[65536] = {0};
		ifsPage.read(buf, length);
		string doc = convert2string(buf);

		WebPage webPage(doc, _jieba);
		_pageLib.insert(std::make_pair(docid, webPage));
	}
	ifsOffset.close();
	ifsPage.close();

	while(getline(ifsInvert, line))
	{
		istringstream iss(line);
		string word;
		vector<string> vecStr;
		while(iss >> word)
		{
			vecStr.push_back(word);
		}
#if 0
		cout << keyWord << " " << docid << " " << weightValue << endl;
		if(docid)
			exit(0);
#endif
		string keyWord = vecStr[0];
		for(size_t idx = 3; idx < vecStr.size(); idx += 4)
		{
			int docid = string2int(vecStr[idx]);
			double weightValue = string2double(vecStr[idx + 1]);
			_invertIndexTable[keyWord].insert(std::make_pair(docid, weightValue));
		}
	}
	ifsInvert.close();
}

vector<double> WordQuery::getQueryWordsWeightVector(vector<string> & queryWords)
{
	map<string, int> wordFreqMap;	//统计词频
	for(auto & elem : queryWords)
	{
		++wordFreqMap[elem];
	}

	map<string, double> queryWordsWeightMap;//使用map可以提前将关键词进行排序
	double weightValueSum = 0;
	for(auto & elem : wordFreqMap)
	{
		int tf = elem.second;
		int df = _invertIndexTable[elem.first].size() + 1;
		int N = _pageLib.size() + 1;

		double idf = ::log2((double)N/(double)df);
		double w = (double)tf * idf;
		queryWordsWeightMap[elem.first] = w;

		weightValueSum += w * w;
	}

	vector<double> queryWordsWeightVec;
	for(auto & elem : queryWordsWeightMap)
	{
		double w = elem.second;
		double W = w/sqrt(weightValueSum);
		queryWordsWeightVec.push_back(W);
	}
	return queryWordsWeightVec;
}

bool WordQuery::executeQuery(const vector<string> & queryWords, MapVectorPair & resultMap)
{
	set<string> queryWordsSet(queryWords.begin(), queryWords.end());//相当于一个去重效果
	//将所有至少有一个查询关键词的docid存储在vector中
	vector<int> queryWordsDocIdVec;			  
	for(auto & elem : queryWordsSet)
	{
		auto it = _invertIndexTable.find(elem);
		for(auto & setElem : it->second)
		{
			queryWordsDocIdVec.push_back(setElem.first);
		}
	}

	set<int> queryWordsDocIdSet;//用于存储每个文档中都包含所有关键词的文档集,用set以便于find查询
	if(queryWords.size() > 1)
	{
		//将vector中重复的docid提取出来
		std::sort(queryWordsDocIdVec.begin(), queryWordsDocIdVec.end());
		auto itCur = queryWordsDocIdVec.begin();
		auto itResult = itCur;
		while(++itCur != queryWordsDocIdVec.end())
		{
			if(*itResult == *itCur)
			{
				queryWordsDocIdSet.insert(*itResult);
			}
			++itResult;
		}

		//遍历每个查询关键词的docid,如果都能在重复docid集找到,说明这些doc包含了所有关键词
		for(auto & elem : queryWordsSet)
		{
			auto it = _invertIndexTable.find(elem);
			
			bool hasRepetition = false;
			for(auto & setElem : it->second)
			{
				auto it = queryWordsDocIdSet.find(setElem.first);
				if(it != queryWordsDocIdSet.end())
				{
					hasRepetition = true;
				}
			}
			if(!hasRepetition)	//遍历后发现有文档中没有交集词,认为查询词错误
			{
				return false;
			}
		}
	}// end of if()
	else//对只有一个查询关键词进行处理
	{
		set<int> tmpSet(queryWordsDocIdVec.begin(), queryWordsDocIdVec.end());	
		queryWordsDocIdSet.swap(tmpSet);
	}

	//将查询关键词与公共文档集进行映射,并
	//可以将键值排序,与查询关键词的权重值一一对应
	//一篇与后期进行计算余弦相似度
	map<string, set<int> > queryWordsDocIdMap;
	for(auto & elem :queryWordsSet)
	{
		set<int> tmp = queryWordsDocIdSet;	
		queryWordsDocIdMap[elem].swap(tmp);
	}

	//将关键词的权重值找出来
	for(auto & elem : queryWordsDocIdMap)			
	{
		string queryWord = elem.first;
		set<int> & docidSet = elem.second;			//queryWord对应的文档集是公共文档集

		auto it = _invertIndexTable.find(queryWord);//在倒排索引中找到查询词(用于找到对应的权重值)
		for(auto & setElem : it->second)			//在查询词对应的docid-权重值集中查找
		{
			auto it = docidSet.find(setElem.first);
			if(it != docidSet.end())				//当与公共文档集匹配 
			{
				resultMap[setElem.first].push_back(setElem.second); 
			}
		}
	}// end of for()
	return true;
}

string WordQuery::createJson(vector<int> & docIdVec, const vector<string> & queryWords)
{
	Json::Value root;
	Json::Value files;

	int cnt = 0;
	for(auto docId : docIdVec)
	{
		auto it = _pageLib.find(docId);
		Json::Value item;
		item["title"] = it->second.getTitle();
		item["summary"] = it->second.summary(queryWords); 
		item["url"] = it->second.getUrl();
		files.append(item);
		if(++cnt == 100)//最多纪录100条
			break;
	}

	root["files"] = files;
	Json::StyledWriter style;//格式化后的Json
	return style.write(root);
}

string WordQuery::returnNoAnswer()
{
	Json::Value root;
	Json::Value arr;

	Json::Value elem;
	elem["title"] = "404, not found";
	elem["summary"] = "亲, I connot find what you want";
	elem["url"] = "";
	arr.append(elem);
	root["files"] = arr;
	Json::StyledWriter writer;
	return writer.write(root);
}

}// end of namespace sh
