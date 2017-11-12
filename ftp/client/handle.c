#include "func.h"

void upload_file(int sfd,char* name)
{
	train t;
	bzero(&t,sizeof(t));
	t.len=strlen(name);
	strcpy(t.buf,name);
	send(sfd,&t,sizeof(t.len)+t.len,0);
	int fd;
	fd=open(name,O_RDWR);
	if(-1==fd)
	{
		perror("open");
		close(sfd);
		return;
	}
	int len;
	recv(sfd,&len,sizeof(len),0);
	lseek(fd,len,SEEK_SET);
	struct stat f_stat;
	bzero(&f_stat,sizeof(f_stat));
	fstat(fd,&f_stat);
	bzero(&t,sizeof(t));
	t.len=f_stat.st_size-len;
	send(sfd,&t.len,sizeof(t.len),0);
	int ret,total=0;
	while(total<t.len)
	{
		ret=sendfile(sfd,fd,NULL,t.len-total);
		total=total+ret;
	}
	close(fd);
}
void download_file(int sfd)
{
	int len;
	char name[20]={0};
	recv(sfd,&len,sizeof(len),0);
	recv(sfd,name,len,0);
	int fd;
	fd=open(name,O_CREAT|O_RDWR,0600);
	if(-1==fd)
	{
		perror("open");
		close(sfd);
		return;
	}
	struct stat f_stat;
	bzero(&f_stat,sizeof(f_stat));
	fstat(fd,&f_stat);
	len=f_stat.st_size;
	lseek(fd,len,SEEK_SET);
	send(sfd,&len,sizeof(len),0);
	int ret,total=0;
	char buf[1000]={0};
	recv(sfd,&len,sizeof(len),0);
	while(total<len)
	{
		bzero(buf,sizeof(buf));
		ret=recv(sfd,buf,sizeof(buf),0);
		write(fd,buf,ret);
		total=total+ret;
	}
	bzero(buf,sizeof(buf));
	sprintf(buf,"%s %s %s","download",name,"success");
	printf("%s\n",buf);
	close(fd);
}
void get_order_name(char* buf,char* name,int len)
{
	int i,j;
	char str[N];
	bzero(str,sizeof(str));
	for(i=len,j=0;buf[i]!='\n';i++)
	{
		if(buf[i]!=' ')
		{
			str[j]=buf[i];	
			j++;
		}
	}
	strcpy(name,str);
}
