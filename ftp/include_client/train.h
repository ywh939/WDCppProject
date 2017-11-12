#ifndef __TRAIN_H__
#define __TRAIN_H__
#include "head.h"

#define N 50

typedef struct{
	int len;
	char buf[1000];
}train,ptrain;

typedef struct{
	int len;
	char account[10];
	char passwd[512];
}data,*pdata;

#endif
