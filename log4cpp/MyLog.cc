 ///
 /// @file    MyLog.cc
 /// @author  yangwenhao
 /// @date    2017-06-03 17:02:04
 ///
 
#include "MyLog.h"

#include <log4cpp/PatternLayout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Priority.hh>

#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

MyLog * MyLog::_pInstance = NULL;

MyLog * MyLog::getMyLogPtr()
{
	if(NULL == _pInstance)
	{
		_pInstance = new MyLog;
		atexit(destroy);
	}
	return _pInstance;
}

void MyLog::destroy()
{
	if(_pInstance)
		delete _pInstance;
}

MyLog::MyLog()
: _cat(log4cpp::Category::getRoot().getInstance("mycat"))
{
	log4cpp::PatternLayout * ptn1 = new log4cpp::PatternLayout();
	ptn1->setConversionPattern("%d [%p] %c:%m%n");

	log4cpp::PatternLayout * ptn2 = new log4cpp::PatternLayout();
	ptn2->setConversionPattern("%d [%p] %c:%m%n");

	log4cpp::OstreamAppender * os = new log4cpp::OstreamAppender("os", &cout);
	os->setLayout(ptn1);

	log4cpp::FileAppender * file = new log4cpp::FileAppender("file", MYLOG);
	file->setLayout(ptn2);

	_cat.addAppender(os);
	_cat.addAppender(file);
	_cat.setPriority(log4cpp::Priority::DEBUG);
}

MyLog::~MyLog()
{
	log4cpp::Category::shutdown();
}

void MyLog::warn(const char * msg)
{
	_cat.warn(msg);
}

void MyLog::error(const char * msg)
{
	_cat.error(msg);
}

void MyLog::info(const char * msg)
{
	_cat.info(msg);
}

void MyLog::debug(const char * msg)
{
	_cat.debug(msg);
}

void logWarn(const char * msg)
{
	MyLog::getMyLogPtr()->warn(msg);
}

void logError(const char * msg)
{
	MyLog::getMyLogPtr()->error(msg);
}

void logInfo(const char * msg)
{
	MyLog::getMyLogPtr()->info(msg);
}

void logDebug(const char * msg)
{
	MyLog::getMyLogPtr()->debug(msg);
}

}// end of namespace sh
