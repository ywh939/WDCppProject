 ///
 /// @file    Task.h
 /// @author  yangwenhao
 /// @date    2017-06-02 21:51:27
 ///
 
#ifndef __TASK_H__
#define __TASK_H__

#include "TcpConnection.h"

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
	Task(const string & queryWord, const TcpConnectionPtr & conn);
	void execute();

private:
	void response();

private:
	string _queryWord;
	const TcpConnectionPtr & _conn;
};

}// end of namespace sh

#endif
