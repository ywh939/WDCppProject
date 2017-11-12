 ///
 /// @file    FileProcessor.h
 /// @author  yangwenhao
 /// @date    2017-05-27 16:48:53
 ///
 
#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include "tinyxml2.h"

#include <string>
#include <vector>
using std::string;
using std::vector;
using namespace tinyxml2;

namespace sh
{

struct Webpage
{
	string _docid;
	string _link;
	string _title;
	string _content;
};

class FileProcessor
{
public:
	FileProcessor();
	vector<string> & process(int & idx, const string & filename);

private:
	void readDoc(int & idx, XMLElement * firstChild);

private:
	vector<string> _vecFiles;
};

}// end of namespace sh

#endif
