 ///
 /// @file    test.cc
 /// @author  yangwenhao
 /// @date    2017-05-17 22:20:39
 ///
 
#include "Configuration.h"
#include "SpellcorrectServer.h"

#include <iostream>
using std::cout;
using std::endl;

int main(void)
{
	sh::Configuration * conf = sh::Configuration::getConfigurationPtr();

	sh::SpellcorrectServer s(
			stoi(conf->query("buffsize")),
			stoi(conf->query("threadNum")),
			stoi(conf->query("initTime")),
			stoi(conf->query("intervalTime")),
			stoi(conf->query("intervalTime")));//任务数,线程数,定时器启动时间,定时器间隔时间,联想单词数目

	s.start();

	return 0;
}
