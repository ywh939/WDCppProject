 ///
 /// @file    CppJieba.cc
 /// @author  yangwenhao
 /// @date    2017-05-23 20:58:59
 ///
 
#include "CppJieba.h"
#include "Configuration.h"
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

CppJieba::CppJieba()
: _conf(Configuration::getConfigurationPtr())
, _DICT_PATH(_conf->query("jieba.dict.utf8"))
, _HMM_PATH(_conf->query("hmm_model.utf8"))
, _USER_DICT_PATH(_conf->query("user.dict.utf8"))
, _IDF_PATH(_conf->query("idf.utf8"))
, _STOP_WORD_PATH(_conf->query("stop_words.utf8"))
, _jieba(_DICT_PATH,
		_HMM_PATH,
		_USER_DICT_PATH,
		_IDF_PATH,
		_STOP_WORD_PATH)
{
}

VectorString CppJieba::cut(const string & line)
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
