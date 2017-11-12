 ///
 /// @file    Task.h
 /// @author  yangwenhao
 /// @date    2017-06-02 21:51:27
 ///
 
#ifndef __TASK_H__
#define __TASK_H__

#include "TcpConnection.h"
#include "WordQuery.h"

#include <string>
#include <memory>
using std::string;
using std::shared_ptr;

namespace sh
{
class Task;
typedef shared_ptr<Task> TaskPtr;

class Task
{
public:
	Task(const string & msg, 
		 const TcpConnectionPtr & conn, 
		 WordQuery & wordQuery);

	void execute(); 

private:
	void response(const string & reply);

private:
	string _msg;
	const TcpConnectionPtr & _conn;
	WordQuery & _wordQuery;
};

}// end of namespace sh

#endif
