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

Task::Task(const string & msg, 
		   const TcpConnectionPtr & conn,
		   WordQuery & wordQuery)
: _msg(msg)
, _conn(conn)
, _wordQuery(wordQuery)
{
}

void Task::execute()
{
	string reply = _wordQuery.doQuery(_msg);
	response(reply);
}

void Task::response(const string & reply)
{
	_conn->sendInLoop(reply);
}

}// end of namespace sh
