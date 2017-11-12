 ///
 /// @file    CppJieba.h
 /// @author  yangwenhao
 /// @date    2017-05-23 20:30:37
 ///
 
#ifndef __CPPJIEBA_H__
#define __CPPJIEBA_H__

#include "cppjiebaInclude/Jieba.hpp"
#include "SplitTool.h"

#include <string>
#include <vector>
using std::vector;
using std::string;

namespace sh
{

typedef vector<string> VectorString;

class Configuration;

class CppJieba : public SplitTool
{
public:
	CppJieba();

	VectorString cut(const string & line);
	void display();

private:
	VectorString _vecWords;
	Configuration * _conf;
	const string _DICT_PATH;
	const string _HMM_PATH;
	const string _USER_DICT_PATH;
	const string _IDF_PATH;
	const string _STOP_WORD_PATH;
	cppjieba::Jieba _jieba;
};

}// end of namespace sh

#endif
