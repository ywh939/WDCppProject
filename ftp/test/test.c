#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>

int main()
{
	char account[10]="192.168";
	int i=10;
	char buf[10]={0};
	sprintf(buf,"%d",i);
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char* server="localhost";
	char* user="root";
	char* password="123";
	char* database="test";
	char query[1000]="insert into logdata(IP,PORT) values('";
	strcat(query,account);
	strcat(query,"',");
	strcat(query,buf);
	strcat(query,")");
	printf("%s\n",query);
	int t,r;
	conn=mysql_init(NULL);
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("insert success\n");
	}
	mysql_close(conn);
	return 0;
}


