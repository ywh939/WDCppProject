 ///
 /// @file    PageLib.cc
 /// @author  yangwenhao
 /// @date    2017-05-27 16:21:19
 ///
 
#include "PageLib.h"
#include "TaskUtil.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

namespace sh
{

PageLib::PageLib(DirScanner & dirScanner,
				 FileProcessor & fileProcessor)
: _dirScanner(dirScanner)
, _confPtr(Configuration::getConfigPtr())
, _fileProcessor(fileProcessor)
, _totalCount(0)
{}

void PageLib::create()
{
	for(auto & elem : _dirScanner.files())
	{
		_vecFiles = _fileProcessor.process(_totalCount, elem);
	}
}

void PageLib::store()
{
	ofstream ofs(_confPtr->getPath("newfipepage"));
	if(!ofs.good())
	{
		cout << "PageLib: ofs newfipepage error!" << endl;
		exit(EXIT_FAILURE);
	}
	int pos = ofs.tellp();
	for(auto & elem : _vecFiles)
	{
		ofs << elem;
		int poscur = ofs.tellp();
		int length = poscur - pos;

		string::size_type m = elem.find("<docid>");
		string::size_type n = elem.find("</docid>");
		int docid = string2int(elem.substr(m + 7, n - m));

		_offsetLib[docid] = std::make_pair(pos, length);
		pos = poscur;
	}
	ofs.close();

	if(_offsetLib.size())
	{
		ofstream ofs(_confPtr->getPath("newoffset"));
		if(!ofs.good())
		{
			cout << "PageLib: ofs newoffset error!" << endl;
			exit(EXIT_FAILURE);
		}
		for(auto & elem : _offsetLib)
		{
			ofs << elem.first << " : "
				<< elem.second.first << " " 
				<< elem.second.second << endl;
		}
		ofs.close();
	}
}

void PageLib::show()
{
	ifstream ifs(_confPtr->getPath("newfipepage"));
	if(!ifs.good())
	{
		cout << "PageLib: ifs newfipepage error!" << endl;
		exit(EXIT_FAILURE);
	}
	for(auto & elem : _offsetLib)
	{
		ifs.seekg(elem.second.first, ifs.beg);
		char buf[65536] = {0};
		ifs.read(buf, elem.second.second);
		cout << buf << endl;
	}
	ifs.close();
}

}// end of namespace sh
