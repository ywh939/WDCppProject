 ///
 /// @file    GlobalDefine.h
 /// @author  yangwenhao
 /// @date    2017-05-31 17:07:47
 ///
 
#ifndef __GLOBALDEFINE_H__
#define __GLOBALDEFINE_H__

#define FIPEPAGELIB_KEY	"fipepage"
#define OFFSETLIB_KEY	"offset"
#define INVERTINDEX_KEY	"invertIndex"

#define BUFF_SIZE		"buffsize"
#define THREAD_NUM		"threadNum"
#define IP				"ip"
#define PORT			"port"

#define SERVER			"localhost"
#define USER			"root"
#define PASSWORD		"123"
#define DATABASE		"invertIndexTable"

#define INSERT			"insert into WDproject(KeyWord,IdWeight) values('"
#define insertPrefix(keyWord, idWeight) string(INSERT)\
						.append(keyWord).append("','")\
						.append(idWeight).append("')")

#define DELETE			"delete from WDproject where KeyWord='"
#define deletePrefix(keyWord) string(DELETE).append(keyWord).append("'")

#define QUERY			"select * from WDproject where KeyWord='"
#define queryPrefix(keyWord) string(QUERY).append(keyWord).append("'")

#define UPDATE			"update WDproject set idWeight='"
#define UPDATE_APPEND	"' where KeyWord='"
#define updatePrefix(keyWord, idWeight) string(UPDATE)\
						.append(idWeight).append(UPDATE_APPEND)\
						.append(keyWord).append("'")

#endif
