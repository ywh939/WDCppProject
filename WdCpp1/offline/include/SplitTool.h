 ///
 /// @file    SplitTool.h
 /// @author  yangwenhao
 /// @date    2017-05-23 22:19:59
 ///
 
#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include <string>
#include <vector>
#include <memory>
using std::string;
using std::vector;
using std::shared_ptr;

namespace sh
{
class SplitTool;
typedef shared_ptr<SplitTool> SplitToolPtr;

class SplitTool
{
public:
	virtual vector<string> cut(const string & line)=0;

};

}// end of namespace sh

#endif
