 ///
 /// @file    Mysql.cc
 /// @author  yangwenhao
 /// @date    2017-06-03 11:35:19
 ///
 
#include "Mysql.h"

#include <stdlib.h>
#include <iostream>
using std::cout;
using std::endl;

namespace sh
{

Mysql * Mysql::_pInstance = NULL;

Mysql * Mysql::getMysqlPtr()
{
	if(NULL == _pInstance)
	{
		_pInstance = new Mysql;
		atexit(destroy);
	}
	return _pInstance;
}

void Mysql::destroy()
{
	if(_pInstance)
		delete _pInstance;
}

Mysql::Mysql() 
: _conn(mysql_init(NULL))
{
	init();
}

Mysql::~Mysql()
{
	mysql_close(_conn);
}

void Mysql::init()
{
	const char * server = SERVER;
	const char * user = USER;
	const char * password = PASSWORD;
	const char * database = DATABASE;
	if(!mysql_real_connect(_conn, server, user, password, database,0,NULL,0))
	{
		cout << "Error connecting to database:" 
			 << mysql_error(_conn) << endl;
	}else{
		cout << "Connected..." << endl;
	}
}

void mysqlInsertData(const string & insertCommand)
{
	Mysql::getMysqlPtr()->insertData(insertCommand);
}

void mysqlDeleteData(const string & deleteCommand)
{
	Mysql::getMysqlPtr()->deleteData(deleteCommand);
}

void mysqlQueryData(const string & queryCommand)
{
	Mysql::getMysqlPtr()->queryData(queryCommand);
}

void mysqlUpdateData(const string & updateCommand)
{
	Mysql::getMysqlPtr()->updateData(updateCommand);
}

void Mysql::insertData(const string & insertCommand)
{
	execute(insertCommand, false);
}

void Mysql::deleteData(const string & deleteCommand)
{
	execute(deleteCommand, false);
}

void Mysql::queryData(const string & queryCommand)
{
	execute(queryCommand, true);
}

void Mysql::updateData(const string & updateCommand)
{
	execute(updateCommand, false);
}

void Mysql::execute(const string & command, bool isQuery)
{
	const char * query = command.c_str();
	//cout << query << endl;
	int t = mysql_query(_conn, query);
	if(t)
	{
		cout << "Error making query:"
			 << mysql_error(_conn) << endl;
	}else{
		if(!isQuery)
		{
			//_result = "success";
		}
		else
		{
			MYSQL_RES * res;
			MYSQL_ROW row;
			res = mysql_use_result(_conn);
			if(res)
			{
				while((row = mysql_fetch_row(res)) != NULL)
				{
					_result = row[1];//针对只有两列数据而言
				}
			}
			mysql_free_result(res);
		}
	}
}

}// end of namespace sh
