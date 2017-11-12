 /// /// @file    DistinguishCnCharacter.h
 /// @author  yangwenhao
 /// @date    2017-05-24 13:42:28
 ///
 
#ifndef __DISTINGUISHCNCHARACTER_H__
#define __DISTINGUISHCNCHARACTER_H__

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;

namespace sh
{

class DistinguishCnCharacter;
typedef shared_ptr<DistinguishCnCharacter> DistinguishCnCharacterPtr;

class DistinguishCnCharacter
{
public:
	DistinguishCnCharacter();

	bool isCnCharacter(const string & queryWord);

private:
	size_t length(const string & queryWord);

private:
	bool _isCnCharacter;
};

}// end of namespace sh

#endif
