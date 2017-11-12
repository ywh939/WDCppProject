 ///
 /// @file    test.cc
 /// @author  yangwenhao
 /// @date    2017-05-26 19:50:52
 ///
 
#include "Configuration.h"
#include "DirScanner.h"
#include "FileProcessor.h"
#include "PageLib.h"
#include <iostream>
using std::cout;
using std::endl;


int main(void)
{
	sh::Configuration * configPtr = sh::Configuration::getConfigPtr();
	configPtr->init("/home/lenovo/project/wd_cpp2/conf/myconf.conf");

	sh::DirScanner dirScanner;
	dirScanner("rssResource");
	sh::FileProcessor fileProcessor;
	sh::PageLib pageLib(dirScanner, fileProcessor);
	
	pageLib.create();
	pageLib.store();
//	pageLib.show();

	return 0;
}
