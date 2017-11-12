 ///
 /// @file    CppJieba.h
 /// @author  yangwenhao
 /// @date    2017-05-23 20:30:37
 ///
 
#ifndef __CPPJIEBA_H__
#define __CPPJIEBA_H__

#include "./cppjiebaInclude/Jieba.hpp"
#include "Configuration.h"

#include <vector>
using std::vector;

namespace sh
{

class Configuration;

class CppJieba
{
public:
	CppJieba();

	vector<string> & cut(const string & line);
	void display();

private:
	vector<string> _vecWords;
	Configuration * _conf;
	cppjieba::Jieba _jieba;
};

}// end of namespace sh

#endif
