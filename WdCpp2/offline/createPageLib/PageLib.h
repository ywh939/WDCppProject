 ///
 /// @file    PageLib.h
 /// @author  yangwenhao
 /// @date    2017-05-27 16:15:28
 ///
 
#ifndef __PAGELIB_H__
#define __PAGELIB_H__

#include "DirScanner.h"
#include "Configuration.h"
#include "FileProcessor.h"
using std::pair;

namespace sh
{

class PageLib
{
public:
	PageLib(DirScanner & dirScanner, 
			FileProcessor & fileProcessor);

	void create();
	void store();
	void show();

private:
	DirScanner & _dirScanner;
	Configuration * _confPtr;
	FileProcessor & _fileProcessor;

	int _totalCount;
	vector<string> _vecFiles;
	map<int, pair<int, int> > _offsetLib;
};

}// end of namespace sh

#endif
