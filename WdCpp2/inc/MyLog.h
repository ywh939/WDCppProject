 ///
 /// @file    MyLog.h
 /// @author  yangwenhao
 /// @date    2017-06-03 16:57:50
 ///
 
#ifndef __MYLOG_H__
#define __MYLOG_H__

#include "TaskUtil.h"
#include <log4cpp/Category.hh>

namespace sh
{

#define prefix(msg) string("[").append(__FILE__).append(":")\
							   .append(__func__).append(":")\
							   .append(sh::convert2string(__LINE__)).append("]")\
							   .append(msg).c_str()

void logWarn(const char * msg);
void logError(const char * msg);
void logInfo(const char * msg);
void logDebug(const char * msg);

#define LogWarn(msg)	logWarn(prefix(msg))
#define LogError(msg)	logError(prefix(msg))
#define LogInfo(msg)	logInfo(prefix(msg))
#define LogDebug(msg)	logDebug(prefix(msg))

#define MYLOG			"/home/lenovo/project/wd_cpp2/log/mylog.log"


class MyLog
{
public:
	static MyLog * getMyLogPtr();
	static void destroy();

	void warn(const char * msg);
	void error(const char * msg); 
	void info(const char * msg); 
	void debug(const char * msg); 

private:
	MyLog();
	~MyLog();

private:
	log4cpp::Category & _cat;
	static MyLog * _pInstance;
};

}// end of namespace sh

#endif
