 /// /// @file    DistinguishCnCharacter.h
 /// @author  yangwenhao
 /// @date    2017-05-24 13:42:28
 ///
 
#ifndef __DISTINGUISHCNCHARACTER_H__
#define __DISTINGUISHCNCHARACTER_H__

#include <string>
using std::string;

namespace sh
{

class DistinguishCnCharacter
{
public:
	DistinguishCnCharacter();

	bool isCnCharacter(const string & queryWord);

private:
	size_t length(const string & queryWord);
	size_t nBytesCode(const char ch);
};

}// end of namespace sh

#endif
