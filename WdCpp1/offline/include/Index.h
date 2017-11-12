 ///
 /// @file    Index.h
 /// @author  yangwenhao
 /// @date    2017-05-17 10:02:35
 ///
 
#ifndef __INDEX_H__
#define __INDEX_H__

#include <string>
#include <unordered_map>
#include <set>
using std::string;
using std::unordered_map;
using std::set;


namespace sh
{
class Configuration;

class Index
{
public:
	Index();
	void createIndex();

private:
	void createCharSet(); 
	void createIndexMap();
	void writeIndexFile() const;
	string char2string(char ch);

private:
	Configuration * _conf;
	set<char> _charSet;
	unordered_map<string, set<int> > _indexMap;
};

}// end of namespace sh


#endif
