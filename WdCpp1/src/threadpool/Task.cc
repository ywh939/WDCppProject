 ///
 /// @file    Task.cc
 /// @author  yangwenhao
 /// @date    2017-05-17 22:14:51
 ///
 
#include "Task.h"
#include "Dictionary.h"
#include "TaskUtil.h"
#include "MinDistance.h"
#include "DistinguishCnCharacter.h"

#include <iostream>
#include <algorithm>
#include <memory>
using std::cout;
using std::endl;

namespace sh
{

Task::Task(const string & queryWord, const TcpConnectionPtr & conn, int wordNumber)
: _pDict(Dictionary::getDictPtr())
, _queryWord(queryWord)
, _conn(conn)
, _wordNumber(wordNumber)
, _dict(_pDict->getDict())
, _cnDict(_pDict->getCnDict())
, _index(_pDict->getIndex())
{
	_queryWord.replace(_queryWord.size() - 1, 1, "\0");		//将\n替换成\0(接收到的queryWord有\n)
	transform(_queryWord.begin(), _queryWord.end(), _queryWord.begin(), ::tolower);	//将大写换成小写
}

void Task::execute(CachePtr cachePtr)
{
	auto it = cachePtr->getCacheMap().find(_queryWord);		//在子线程的cache缓存中查找是否有queryWord
	if(it == cachePtr->getCacheMap().end())					//如果没有找到,就从建立的在线字典找
	{
		DistinguishCnCharacter distCnChar;			//判断是否中文类
		if(distCnChar.isCnCharacter(_queryWord))	//如果是中文
		{
			cout << "--------queryCnDict---------" << endl;
			queryIndex(true);
		}else{
			cout << "--------queryEnDict---------" << endl;
			queryIndex(true);							//查询索引
		}
		resultPushPriorityQueue();					//将结果集放在优先级队列中,这样可以通过最小编辑距离和词频获取最佳联想
		resultPushCache(cachePtr);					//存放在子线程的cache中
	}
	response(cachePtr);
}

void Task::queryIndex(bool isCnWord)
{
	int pos = 0;
	for(size_t idx = 0; idx != _queryWord.size(); ++idx)
	{
		int nBytes = nBytesCode(_queryWord[idx]);			//定义在TaskUtil.h
		string word = _queryWord.substr(pos, nBytes);
		auto it = _index.find(word);//通过输入单词的第idx个字符,find到索引相应的位置 
		if(it != _index.end())								//char2string()定义在TaskUtil.h中(将字符转为字符串)
		{
			for(int elem : it->second)						//elem为索引里对应的数字
			{
				if(isCnWord)
				{
					if(_queryWord == _cnDict[elem].first)
						_resultVec.push_back(MyResult(_queryWord, _dict[elem].second, 0));//将结果集存在vector
					else
						minDistanceCalculation(_cnDict[elem]);			//当vector没有相同的单词,准备进行联想单词(计算最小编辑距离)
				}
				else
				{
					if(_queryWord == _dict[elem].first)			//通过elem取vector下标,当vector里有单词与输入单词相同
						_resultVec.push_back(MyResult(_queryWord, _dict[elem].second, 0));//将结果集存在vector
					else
						minDistanceCalculation(_dict[elem]);			//当vector没有相同的单词,准备进行联想单词(计算最小编辑距离)
				}
			}
		}
		idx += (nBytes - 1);
		pos += nBytes;
	}// end of for()
}

void Task::minDistanceCalculation(PairType pairDict)
{
	MinDistance minDist(_queryWord, pairDict.first);		//最小距离计算
	minDist.init();
	minDist.statistic();

	_resultVec.push_back(MyResult(pairDict.first,		//将计算后的结果集存入vector
					pairDict.second, 
					minDist.minDistance()));
}

void Task::resultPushPriorityQueue()
{
	sort(_resultVec.begin(), _resultVec.end(), SortCompare());//将vector中的数据按word大小排列,与unique(),erase()组合使用	
	_resultVec.erase(unique(_resultVec.begin(), _resultVec.end(), UniqueCompare()), _resultVec.end());//将vector中重复的数据剔除 

	for(size_t idx = 0; idx != _resultVec.size(); ++idx)
	{
		_resultQue.push(_resultVec[idx]);					//将vector放在优先级队列中,按最小编辑距离和词频排序
	}
}

void Task::resultPushCache(CachePtr cachePtr)
{
	string msg = _resultQue.top()._word;					//先获取优先级队列队头元素	
	_resultQue.pop();										//将对头元素剔除
	for(int idx = 1; idx != _wordNumber; ++idx)				//再获取_wordNumber-1个数据
	{
		msg.append(" ");
		msg.append(_resultQue.top()._word);
		_resultQue.pop();
	}
	cachePtr->addElement(_queryWord, msg);					//将queryWord与获得的联想单词集,存放在cache缓存中
}

void Task::response(CachePtr cachePtr)
{
	_conn->sendInLoop(cachePtr->getCacheMap()[_queryWord]);	//将在工作线程(即线程池)处理的数据msg发送到IO线程(即epoll),由IO线程发送msg
}

}// end of namespace sh
