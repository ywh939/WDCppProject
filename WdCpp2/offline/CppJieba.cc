 ///
 /// @file    CppJieba.cc
 /// @author  yangwenhao
 /// @date    2017-05-23 20:58:59
 ///
 
#include "CppJieba.h"
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

CppJieba::CppJieba()
: _conf(Configuration::getConfigPtr())
, _jieba(_conf->getPath("jieba.dict.utf8"),
		 _conf->getPath("hmm_model.utf8"),
		 _conf->getPath("user.dict.utf8"),
		 _conf->getPath("idf.utf8"),			
	     _conf->getPath("stop_words.utf8"))
{
}

vector<string> & CppJieba::cut(const string & line)
{
	_jieba.Cut(line, _vecWords, true);
//	display();
	return _vecWords;
}

void CppJieba::display()
{
	for(auto & elem : _vecWords)
	{
		cout << elem << " ";
	}
	cout << endl;
}

}//end of namespace sh
