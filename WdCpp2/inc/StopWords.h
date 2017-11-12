 ///
 /// @file    StopWords.h
 /// @author  yangwenhao
 /// @date    2017-05-24 20:16:11
 ///
 
#ifndef __STOPWORDS_H__
#define __STOPWORDS_H__

#include "Configuration.h"
#include <memory>
using std::shared_ptr;

namespace sh
{

class StopWords
{
public:
	StopWords();
	set<string> & getStopWords() {	return _stopWords;	}

private:
	void init();
	void readFromFile(const string filePath, bool isStopCnFile);

private:
	set<string> _stopWords;
	Configuration * _conf;
};

}

#endif
