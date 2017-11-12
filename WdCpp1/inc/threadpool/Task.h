 ///
 /// @file    task.h
 /// @author  yangwenhao
 /// @date    2017-05-11 09:57:53
 ///
 
#ifndef __WD_TASK_H__
#define __WD_TASK_H__

#include "TcpServer.h"
#include "MinDistance.h"
#include "Cache.h"

#include <string>
#include <queue>
#include <vector>
#include <set>
#include <memory>
using std::string;
using std::priority_queue;
using std::vector;
using std::pair;
using std::set;
using std::shared_ptr;

namespace sh
{

struct MyResult
{
	MyResult(string word, int freq, int dist)
	: _word(word)
	, _iFreq(freq)
	, _iDist(dist)
	{}

	string _word;//候选词
	int _iFreq;	//词频
	int _iDist;	//与查询词的最小编辑距离
};

struct SortCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{
		return lhs._word < rhs._word;
	}
};

struct UniqueCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{
		return lhs._word == rhs._word;
	}
};

struct MyCompare
{
	bool operator()(const MyResult & lhs, const MyResult & rhs)
	{  
		if(lhs._iDist > rhs._iDist)		//当队头元素比新加入元素大时,将新元素放在队头(即队头最小,队尾最大) 
			return true;
		else if(lhs._iDist == rhs._iDist)
			return lhs._iFreq < rhs._iFreq;//词频高的放在队头
		else
			return false;
	}
};


class Dictionary;
class Task;

typedef shared_ptr<Task> TaskPtr;
typedef pair<string, int> PairType;

class Task
{
public:
	Task(const string & queryWord, 
		 const TcpConnectionPtr & conn, 
		 int wordNumber);

	void execute(CachePtr cachePtr);

private:
	void queryIndex(bool isCnWord);				
	void minDistanceCalculation(PairType pairDict);		
	void resultPushPriorityQueue();
	void resultPushCache(CachePtr cachePtr);
	void response(CachePtr cachePtr);					

private:
	Dictionary * _pDict;

	string _queryWord;
	const TcpConnectionPtr & _conn;
	int _wordNumber;

	const vector<PairType> & _dict;
	const vector<PairType> & _cnDict;
	const map<string, set<int> > & _index;

	vector<MyResult> _resultVec;
	priority_queue<MyResult, vector<MyResult>, MyCompare> _resultQue;
};

}// end of namespace sh

#endif
