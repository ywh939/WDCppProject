 ///
 /// @file    testServer.cc
 /// @author  yangwenhao
 /// @date    2017-06-02 22:12:35
 ///
 
#include "Configuration.h"
#include "CommonServer.h"


int main(void)
{
	sh::Configuration * _conf = sh::Configuration::getConfigPtr();
	_conf->init("/home/lenovo/project/wd_cpp2/conf/myconf.conf");

	sh::CommonServer commonServer;

	commonServer.start();
}
