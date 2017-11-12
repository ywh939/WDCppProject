 ///
 /// @file    min.h
 /// @author  yangwenhao
 /// @date    2017-05-19 14:39:49
 ///
 
#ifndef __MIN_H__
#define __MIN_H__

#include <vector>
#include <string>
using std::vector;
using std::string;


namespace sh
{

typedef vector<vector<int> > vectorType;
typedef vectorType::size_type size_t;

class MinDistance
{
public:
	MinDistance(const string & lhs, const string & rhs);

	void init();
	void display();
	int minDistance();
	void statistic();

private:
	size_t nBytesCode(const char ch);
	size_t length(const string & str);
	int triple_min(const int & a, const int & b, const int & c);

	int minDist(int a, int b, int c);
	int compare(const string & lhs, const string & rhs);
private:
	const string & _lhs;
	const string & _rhs;
	size_t _row;
	size_t _column;
	vectorType _vec;
};

}// end of namespace sh

#endif 
