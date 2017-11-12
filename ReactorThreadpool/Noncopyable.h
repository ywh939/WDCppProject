 ///
 /// @file    Noncopyable.h
 /// @author  yangwenhao
 /// @date    2017-05-10 20:08:22
 ///
 
#ifndef __WD_NONCOPYABLE_H__
#define __WD_NONCOPYABLE_H__

namespace sh
{

class Noncopyable
{
protected:
	Noncopyable(){}
	~Noncopyable(){}
private:
	Noncopyable(const Noncopyable&);
	Noncopyable operator=(const Noncopyable&);
};

}// end of namesapce sh

#endif
