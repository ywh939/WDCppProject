 ///
 /// @file    TaskUtil.h
 /// @author  yangwenhao
 /// @date    2017-05-18 22:14:32
 ///
 
#ifndef __TASKUTIL_H__
#define __TASKUTIL_H__

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;

namespace sh
{

inline string char2string(char ch)
{
	ostringstream oss;
	oss << ch;
	return oss.str();
}

inline size_t nBytesCode(const char ch)
{
	if(ch & 1 << 7)				//得到1,至少有两个字节,再接着进行低位与操作
	{
		int nBytes = 1;
		for(size_t idx = 0; idx != 6; ++idx)
		{
			if(ch & 1 << (6 - idx))
				++nBytes;
			else
				break;
		}
		return nBytes;
	}
	return 1;					//ch,10000000进行与操作,得到0,则只要一个字节
}

}//end of namespace sh

#endif
