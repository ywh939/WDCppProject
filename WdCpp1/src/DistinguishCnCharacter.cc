 ///
 /// @file    DistinguishCnCharacter.cc
 /// @author  yangwenhao
 /// @date    2017-05-24 13:49:08
 ///
 
#include "DistinguishCnCharacter.h"
#include "TaskUtil.h"

namespace sh
{

DistinguishCnCharacter::DistinguishCnCharacter()
: _isCnCharacter(false)
{
}

bool DistinguishCnCharacter::isCnCharacter(const string & queryWord)
{
	if(queryWord.size() != length(queryWord))
		_isCnCharacter = true;

	return _isCnCharacter;
}

size_t DistinguishCnCharacter::length(const string & queryWord)
{
	size_t ilen = 0;
	for(size_t idx = 0; idx != queryWord.size(); ++idx)
	{
		int nBytes = nBytesCode(queryWord[idx]);			//定义在TaskUtil.h
		idx += (nBytes - 1);			//for()循环里面也进行了++,故这里要-1;
		++ilen;
	}
	return ilen;
}

}// end of namespace sh
