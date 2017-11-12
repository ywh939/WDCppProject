 ///
 /// @file    min.cc
 /// @author  yangwenhao
 /// @date    2017-05-19 13:29:02
 ///
 
#include "MinDistance.h"
#include <iostream>
using std::cout;
using std::endl;


namespace sh
{

MinDistance::MinDistance(const string & lhs, const string & rhs)
: _lhs(lhs)
, _rhs(rhs)
, _row(length(lhs) + 1)
, _column(length(rhs) + 1)
, _vec(_row, vector<int>(_column, 0)) 
{
}

void MinDistance::init()
{
	for(int idx = 0; idx != _row; ++idx)
	{
		_vec[idx][0] = idx;
	}
	for(int idx = 0; idx != _column; ++idx)
	{
		_vec[0][idx] = idx;
	}
}

void MinDistance::display()
{
	for(size_t i = 0; i != _vec.size(); ++i)
	{
		for(size_t j = 0; j != _vec[0].size(); ++j)
		{
			cout << _vec[i][j] << " ";
		}
		cout << endl;
	}
}

size_t MinDistance::length(const string & str)
{
	size_t ilen = 0;
	for(size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);			//for()循环里面也进行了++,故这里要-1;
		++ilen;
	}
	return ilen;
}

size_t MinDistance::nBytesCode(const char ch)
{
	if(ch & (1 << 7))				//UTF8编码规则如果与后为真,则为2个字节以上,负责返回1
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)//再接着与后面几位
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;
			}
			else
				break;				//当发现为0时退出
		}
		return nBytes;
	}
	return 1;
}

int MinDistance::minDistance()
{
	return _vec[_row - 1][_column - 1];
}

void MinDistance::statistic()
{
	string sublhs, subrhs;
	for(size_t dist_i = 1, lhs_idx = 0; dist_i != _row; ++dist_i, ++lhs_idx)//0行0列不进行计算,故从1开始,lhs_idx是操作数字
	{
		size_t nBytes = nBytesCode(_lhs[lhs_idx]);	//获得_lhs第一个字符所占字节数
		sublhs = _lhs.substr(lhs_idx, nBytes);		//获得nBytes字节数所代表的字符
		lhs_idx += (nBytes - 1);					//下一次直接找nBytes字节后的字符

		for(size_t dist_j = 1, rhs_idx = 0; dist_j != _column; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(_rhs[rhs_idx]);
			subrhs = _rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);

#if 0
			_vec[dist_i][dist_j] = minDist(_vec[dist_i - 1][dist_j] + 1, 
						_vec[dist_i - 1][dist_j - 1] + compare(sublhs, subrhs),
						_vec[dist_i][dist_j - 1] + 1);
#endif
#if 1
			if(sublhs == subrhs)
			{
				_vec[dist_i][dist_j] = _vec[dist_i - 1][dist_j - 1];
			}
			else
			{
				_vec[dist_i][dist_j] = triple_min(
						_vec[dist_i][dist_j - 1] + 1,
						_vec[dist_i - 1][dist_j] + 1,
						_vec[dist_i - 1][dist_j - 1] + 2);
			}
#endif
		}// end of for()
	}// end of for()
}

int MinDistance::triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int MinDistance::minDist(int a, int b, int c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int MinDistance::compare(const string & lhs, const string & rhs)
{
	return lhs == rhs ? 0 : 2;
}

}// end of namespace sh
