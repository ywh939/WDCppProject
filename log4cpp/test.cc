 ///
 /// @file    test.cc
 /// @author  yangwenhao
 /// @date    2017-06-03 17:14:47
 ///
 
#include "MyLog.h"
#include <iostream>
using std::cout;
using std::endl;

void test()
{
	sh::LogWarn("warn message");
	sh::LogError("warn message");
	sh::LogInfo("warn message");
	sh::LogDebug("warn message");
}

int main(void)
{
	test();

	return 0;
}
