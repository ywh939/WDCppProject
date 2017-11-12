 ///
 /// @file    DirScanner.h
 /// @author  yangwenhao
 /// @date    2017-05-27 15:24:53
 ///
 
#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <vector>
#include <string>
using std::vector;
using std::string;

namespace sh
{

class DirScanner
{
public:
	DirScanner();
	vector<string> & files()	{	return _vecFiles;	}
	void operator()(const string & dirname);
	void showVecFiles();

private:
	void traverse(const string & dirname);

private:
	vector<string> _vecFiles;
};

}// end of namespace sh

#endif
