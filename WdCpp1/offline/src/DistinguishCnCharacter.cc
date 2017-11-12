 ///
 /// @file    DistinguishCnCharacter.cc
 /// @author  yangwenhao
 /// @date    2017-05-24 13:49:08
 ///
 
#include "DistinguishCnCharacter.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

DistinguishCnCharacter::DistinguishCnCharacter()
{
}

bool DistinguishCnCharacter::isCnCharacter(const string & queryWord)
{
	if(queryWord.size() != length(queryWord))
		return true;					//代表是中文形式字符
	else
		return false;
}

size_t DistinguishCnCharacter::length(const string & queryWord)
{
	size_t ilen = 0;
	for(size_t idx = 0; idx != queryWord.size(); ++idx)
	{
		int nBytes = nBytesCode(queryWord[idx]);
		idx += (nBytes - 1);			//for()循环里面也进行了++,故这里要-1;
		++ilen;
	}
	return ilen;
}

size_t DistinguishCnCharacter::nBytesCode(const char ch)
{
	if(ch & (1 << 7))				//UTF8编码规则如果与后为真,则为2个字节以上,负责返回1
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)//再接着与后面几位
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;
			}
			else
				break;				//当发现为0时退出
		}
		return nBytes;
	}
	return 1;
}

}// end of namespace sh
