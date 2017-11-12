 ///
 /// @file    PageLibPreprocessor.h
 /// @author  yangwenhao
 /// @date    2017-05-29 11:37:47
 ///
 
#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include "Configuration.h"
#include "WebPage.h"
#include <unordered_map>
using std::unordered_map;
using std::pair;

namespace sh
{

class PageLibPreprocessor
{
public:
	PageLibPreprocessor();
	void doProcess();

private:
	void readInfoFromFile();
	void cutRedundantPages();
	void builtInvertIndexTable();
	void storeOnDisk();

private:
	Configuration * _conf;
	CppJieba _jieba;
	vector<WebPage> _pageLib;
	unordered_map<int, pair<int, int> > _offsetLib;
	unordered_map<string, set<pair<int, double> > > _invertIndexTable;
};

}// end of namespace sh

#endif
