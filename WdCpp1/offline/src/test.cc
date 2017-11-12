 ///
 /// @file    test.cc
 /// @author  yangwenhao
 /// @date    2017-05-23 21:56:06
 ///
 
#include "DictProducer.h"
#include "Index.h"

#include <iostream>
using std::cin;

int main(void)
{
	sh::DictProducer dict;
	dict.built_cn_dict();
	dict.built_en_dict();
	
//	sh::Index index;
//	index.createIndex();

	return 0;
}
