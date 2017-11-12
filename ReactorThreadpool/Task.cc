 ///
 /// @file    Task.cc
 /// @author  yangwenhao
 /// @date    2017-06-02 22:00:56
 ///
 
#include "Task.h"
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

Task::Task(const string & queryWord, const TcpConnectionPtr & conn)
: _queryWord(queryWord)
, _conn(conn)
{
}

void Task::execute()
{
	response();
}

void Task::response()
{
	_conn->sendInLoop(_queryWord);
}

}// end of namespace sh
