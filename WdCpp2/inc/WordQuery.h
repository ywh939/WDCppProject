 ///
 /// @file    WordQuery.h
 /// @author  yangwenhao
 /// @date    2017-05-31 10:24:58
 ///
 
#ifndef __WORDQUERY_H__
#define __WORDQUERY_H__

#include "Configuration.h"
#include "WebPage.h"

#include <unordered_map>
using std::unordered_map;

namespace sh
{

typedef map<int, vector<double> > MapVectorPair;

class WordQuery
{
public:
	WordQuery();

	string doQuery(const string & str);

private:
	void loadLibrary();
	vector<double> getQueryWordsWeightVector(vector<string> & queryWords);
	bool executeQuery(const vector<string> & queryWords, MapVectorPair & resultVec);
	string createJson(vector<int> & docIdVec, const vector<string> & queryWords);
	string returnNoAnswer();

private:
	Configuration * _conf;
	CppJieba _jieba;
	
	unordered_map<int, WebPage> _pageLib;
	unordered_map<int, pair<int, int> > _offsetLib;
	unordered_map<string, set<pair<int, double> > > _invertIndexTable;
};

}// end of namespace sh

#endif
