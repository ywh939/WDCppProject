 ///
 /// @file    Mysql.h
 /// @author  yangwenhao
 /// @date    2017-06-03 10:36:39
 ///
 
#ifndef __MYSQL_H__
#define __MYSQL_H__

#include "GlobalDefine.h"
#include <mysql/mysql.h>

#include <string>
using std::string;

namespace sh
{

void mysqlInsertData(const string & insertCommand);
void mysqlDeleteData(const string & deleteCommand);
void mysqlQueryData(const string & queryCommand);
void mysqlUpdateData(const string & updateCommand);

#define MysqlInsert(keyWord, idWeight) mysqlInsertData(insertPrefix(keyWord, idWeight))
#define MysqlDelete(keyWord)		   mysqlDeleteData(deletePrefix(keyWord))
#define MysqlQuery(keyWord)			   mysqlQueryData(queryPrefix(keyWord))
#define MysqlUpdate(keyWord, idWeight) mysqlUpdateData(updatePrefix(keyWord, idWeight))

class Mysql
{
public:
	static Mysql * getMysqlPtr();
	static void destroy();

	const string & getResult() {	return _result;	}

	void insertData(const string & insertCommand);
	void deleteData(const string & deleteCommand);
	void queryData(const string & queryCommand);
	void updateData(const string & updateCommand);

private:
	Mysql();
	~Mysql();

	void init();
	void execute(const string & command, bool isQuery); 

private:
	static Mysql * _pInstance;
	MYSQL * _conn;
	string _result;
};

}// end of namespace sh

#endif
