 ///
 /// @file    WebPage.cc
 /// @author  yangwenhao
 /// @date    2017-05-28 16:56:31
 ///
 
#include "WebPage.h"
#include "TaskUtil.h"
#include "stdlib.h"
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;

namespace sh
{

bool operator==(const WebPage & lhs, const WebPage & rhs)
{
	int k = 0;
	for(size_t i = 0; i != lhs._topWords.size(); ++i)
	{
		for(size_t j = 0; j != lhs._topWords.size(); ++j)
		{
			if(lhs._topWords[i] == rhs._topWords[j])
			{
				++k;
			}
		}
	}
	if(k > 6)//如果有6个词语都相同，认为该文档重了
		return true;
	else
		return false;
}

bool operator<(const WebPage & lhs, const WebPage & rhs)
{
	if(lhs._docId < rhs._docId)
		return true;
	else
		return false;
}

WebPage::WebPage(string & doc, CppJieba & jieba)
: _stopWords(StopWords())
, _doc(doc)
{
	processDoc(jieba);
}

void WebPage::processDoc(CppJieba & jieba)
{
	string::size_type m;
	string::size_type n;

	m = _doc.find("<docid>");
	n = _doc.find("</docid>");
	_docId = string2int(_doc.substr(m + 7, n - (m + 7)));//7 = sizeof("<docid>") - 1 (将末尾\0去掉)
	m = _doc.find("<title>");
	n = _doc.find("</title>");
	_docTitle = _doc.substr(m + 7, n - (m + 7));
	m = _doc.find("<link>");
	n = _doc.find("</link>");
	_docLink = _doc.substr(m + 6, n - (m + 6));
	m = _doc.find("<content>");
	n = _doc.find("</content>");
	_docContent = _doc.substr(m + 9, n - (m + 9));	

	vector<string> vec= jieba.cut(_docContent);
	calcTopK(vec); 
}

void WebPage::calcTopK(vector<string> & wordsVec)
{
	set<string> & stopWordList = _stopWords.getStopWords();
	for(auto & elem : wordsVec)
	{
		auto it = stopWordList.find(elem);
		if(it != stopWordList.end())
			continue;
		else if(elem[0] >= 0 && elem[0] <= 32)
			continue;
		else
		{
			++_wordsMap[elem];
		}
	}

	if(_wordsMap.size() >= TOPK_NUMBER)//判断网页内容全部是图片?全部是认为小于10
	{
		vector<Pair> wordsVec(_wordsMap.begin(), _wordsMap.end());
		std::sort(wordsVec.begin(), wordsVec.end(), CmpByValue());
		for(size_t idx = 0; idx != TOPK_NUMBER; ++idx)
		{
			_topWords.push_back(wordsVec[idx].first);
		}
	}
}

}// end of namespace sh
