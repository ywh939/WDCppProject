 ///
 /// @file    WebPage.h
 /// @author  yangwenhao
 /// @date    2017-05-28 16:48:32
 ///
 
#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include "Configuration.h"
#include "StopWords.h"
#include "CppJieba.h"

#include <vector>
using std::vector;
using std::pair;

namespace sh
{
typedef pair<string, int> Pair;

struct CmpByValue
{
	bool operator()(const Pair & lhs, const Pair rhs)
	{
		return lhs.second > rhs.second;
	}
};

class WebPage
{
	friend bool operator==(const WebPage & lhs, const WebPage & rhs);
	friend bool operator<(const WebPage & lhs, const WebPage & rhs);
public:
	WebPage(string & doc, CppJieba & jieba);
	
	string getDoc()		{	return _doc;	}

	int getDocId()		{	return _docId;	}
	string getTitle()	{	return _docTitle;	}
	string getUrl()		{	return _docLink;	}
	string getContent()	{	return _docContent;	}

	map<string, int> & getWordsMap() {	return _wordsMap;	}

	string summary(const vector<string> & queryWords);

private:
	void processDoc(CppJieba & jieba);
	void calcTopK(vector<string> & wordsVec);

private:
	const static int TOPK_NUMBER = 10;
	StopWords _stopWords;
	string _doc;

	int _docId;
	string _docTitle;
	string _docLink;
	string _docContent;

	vector<string> _topWords;
	map<string, int> _wordsMap;
};

}// end of namespace sh

#endif
