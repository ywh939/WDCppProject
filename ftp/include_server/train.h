#ifndef __TRAIN_H__
#define __TRAIN_H__
#include "head.h"

#define N 50

typedef struct{
	pid_t pid;
	int fdw;
	short stat;
}ctrl,*pctrl;

typedef struct{
	int len;
	char buf[1000];
}train,ptrain;

#endif
