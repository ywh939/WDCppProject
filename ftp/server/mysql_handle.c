#include "func.h"

void insert_mysql_logdata(char*ip,int port,char* command,char* time)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="ftp";
	char query[1000]="insert into logdata(IP,PORT,linktime,handle,handletime) values('";
	strcat(query,ip);
	strcat(query,"',");
	char pt[10]={0};
	sprintf(pt,"%d",port);
	strcat(query,pt);
	strcat(query,",'");
	strcat(query,time);
	strcat(query,"','");
	strcat(query,command);
	strcat(query,"','");
	strcat(query,time);
	strcat(query,"')");
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		//printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		//printf("insert success\n");
	}
	mysql_close(conn);
}

void query_mysql(int* i,char* buf,char* acc,char* sa,char* pa)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="ftp";
	char* query="select * from ftpdata";
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		//printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
			printf("Error making query:%s\n",mysql_error(conn));
	}else{
		//printf("Query made...\n");
		res=mysql_use_result(conn);
		if(res)
		{
			while((row=mysql_fetch_row(res))!=NULL)
			{	
				//printf("num=%d\n",mysql_num_fields(res));//列数
				for(t=0;t<mysql_num_fields(res);t++)
				{
					if(0==strcmp(row[t],buf))
					{
						*i=1;
						strcpy(acc,row[0]);
						strcpy(sa,row[1]);
						strcpy(pa,row[2]);
						break;
					}
				}
			}
		}
		mysql_free_result(res);
	}
	mysql_close(conn);
}

void insert_mysql_ftpdata(char* account,char* salt,char* passwd)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="ftp";
	char query[1000]="insert into ftpdata(username,saltvalue,passward) values('";
	strcat(query,account);
	strcat(query,"','");
	strcat(query,salt);
	strcat(query,"','");
	strcat(query,passwd);
	strcat(query,"')");
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		//printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		//printf("insert success\n");
	}
	mysql_close(conn);
}
