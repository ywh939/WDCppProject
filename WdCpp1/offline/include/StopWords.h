 ///
 /// @file    StopWords.h
 /// @author  yangwenhao
 /// @date    2017-05-24 20:16:11
 ///
 
#ifndef __STOPWORDS_H__
#define __STOPWORDS_H__

#include <string>
#include <set>
using std::string;
using std::set;

namespace sh
{

typedef set<string> StopWordsSet;

class Configuration;
class SplitTool;

class StopWords
{
public:
	StopWords();

	StopWordsSet & getStopWords(bool isStopCnWords);

private:
	void init();
	void readFromFile(const string filePath, bool isStopCnFile);

private:
	Configuration * _conf;

	StopWordsSet _stopEnWords;
	StopWordsSet _stopCnWords;
};

}

#endif
