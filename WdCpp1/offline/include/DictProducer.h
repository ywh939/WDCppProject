 ///
 /// @file    DictProducer.h
 /// @author  yangwenhao
 /// @date    2017-05-23 22:17:39
 ///
 
#ifndef __DICTPRODUCER_H__
#define __DICTPRODUCER_H__

#include "DistinguishCnCharacter.h"
#include "StopWords.h"

#include <string>
#include <memory>
#include <vector>
#include <map>
using std::string;
using std::shared_ptr;
using std::vector;
using std::map;

namespace sh
{

class Configuration;
class SplitTool;

typedef shared_ptr<SplitTool> SplitToolPtr;

class DictProducer
{
public:
	DictProducer();

	void built_en_dict();
	void built_cn_dict();

private:
	void pushDict(bool isEnDict);
	void builtFilePathVector(const string & str);
	void storeDictToDisk(const string dictPath);

private:
	Configuration * _conf;

	const string _enYuliaoDir;
	const string _cnYuliaoDir;
	SplitToolPtr _splitToolPtr;
	DistinguishCnCharacter _distCnChar;
	StopWords _stopWords;

	vector<string> _vecFilePath;
	vector<string> _vecWords;
	map<string, int> _mapDict;
};

}// end of namespace sh

#endif
