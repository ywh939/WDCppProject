 ///
 /// @file    Dictionary.h
 /// @author  yangwenhao
 /// @date    2017-05-18 11:42:36
 ///
 
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::set;
using std::map;
using std::pair;

namespace sh
{
class Configuration;
typedef vector<pair<string, int> > VectorDict;

class Dictionary
{
private:
	class AutoRelease
	{
	public:
		AutoRelease() {}
		~AutoRelease();
	};
public:
	static Dictionary * getDictPtr();

	VectorDict & getDict()		
	{ return _dict; }

	VectorDict & getCnDict() 
	{ return _cnDict; }
	
	const map<string, set<int> > & getIndex() 
	{ return _index; }

	void showIndexTable();

private:
	Dictionary();
	~Dictionary() {}

	void init();
	void initDict(const string dictName, bool isCnDict);
	void initIndex(VectorDict & vec);

private:
	Configuration * _conf;

	static Dictionary * _pDict;
	static AutoRelease _autoRelease;
	VectorDict _dict;
	VectorDict _cnDict;
	map<string, set<int> > _index;
};


}// end of namespace sh

#endif
