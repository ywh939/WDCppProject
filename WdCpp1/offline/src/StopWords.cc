 ///
 /// @file    StopWords.cc
 /// @author  yangwenhao
 /// @date    2017-05-24 20:25:58
 ///
 
#include "StopWords.h"
#include "Configuration.h"
#include "SplitTool.h"
#include "CppJieba.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

namespace sh
{

StopWords::StopWords()
: _conf(Configuration::getConfigurationPtr())
{
	init();
}

StopWordsSet & StopWords::getStopWords(bool isStopCnWords)
{
	if(isStopCnWords)
		return _stopCnWords;
	else
		return _stopEnWords;
}

void StopWords::init()
{
	readFromFile(_conf->query("stop_words_cn"), true);
	readFromFile(_conf->query("stop_words_en"), false);
}

void StopWords::readFromFile(const string filePath, bool isStopCnFile)
{
	ifstream ifs(filePath);
	if(!ifs.good())
	{
		cout << "StopWords: ifs stop_words error" << endl;
		exit(EXIT_FAILURE);
	}

	string line;
	string word;
	while(getline(ifs, line))
	{
		istringstream iss(line);
		iss >> word;
		if(isStopCnFile)
			_stopCnWords.insert(word);
		else
			_stopEnWords.insert(word);
	}
}

}// end of namespace sh
