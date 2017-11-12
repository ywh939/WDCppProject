 ///
 /// @file    test.cc
 /// @author  yangwenhao
 /// @date    2017-06-03 14:32:17
 ///
 
#include "Mysql.h"

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;


int main(int argc, char * argv[])
{
	sh::Mysql * mysql = sh::Mysql::getMysqlPtr();

#if 0
	cout << "insert..." << endl;
	sh::MysqlInsert("深圳", "(4, 0.00851) (3289, 0.031)");
#endif

#if 0
	cout << "delete..." << endl;
	sh::MysqlDelete("深圳");
#endif

#if 0
	cout << "update..." << endl;
	sh::MysqlUpdate("深圳", "(429, 0.00519) (289, 0.04931510)");
#endif

#if 1
	cout << "query..." << endl;
	sh::MysqlQuery("深圳");
	cout << mysql->getResult() << endl;
#endif

	return 0;
}
