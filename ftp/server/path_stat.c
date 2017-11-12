#include "func.h"

void stat_mode(char* mode,int md)
{
	int i;
	int m[20]={0};
	for(i=0;md;i++)		//将十进制的文件类型转为二进制
	{
		m[i]=md%2;
		md=md/2;
	}
	if(15==i)
	{
		strcpy(mode,"d");
	}
	if(16==i)
	{
		strcpy(mode,"-");
	}
}

void get_stat(char* data,char* path,char* file_name)
{
	char path_name[50]={0};
	sprintf(path_name,"%s%s%s",path,"/",file_name);
	struct stat buf;
	int ret=stat(path_name,&buf);
	if(ret!=0)
	{
		perror("stat");
		return;
	}
	char mode[2]={0};
	stat_mode(mode,buf.st_mode);
	char ls_data[100]={0};
	char buf_size[10]={0};
	sprintf(buf_size,"%ld",buf.st_size);
	int len=30-strlen(file_name)-strlen(buf_size);
	sprintf(ls_data,"%s %s%*s%ld",mode,file_name,len,"",buf.st_size); 
	strcpy(data,ls_data);
}
