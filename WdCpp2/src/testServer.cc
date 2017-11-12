 ///
 /// @file    testServer.cc
 /// @author  yangwenhao
 /// @date    2017-06-02 22:12:35
 ///
 
#include "Configuration.h"
#include "WordQueryServer.h"


int main(void)
{
	sh::Configuration * conf = sh::Configuration::getConfigPtr();
	conf->init("/home/lenovo/project/wd_cpp2/conf/myconf.conf");

	sh::WordQueryServer server;

	server.start();
}
