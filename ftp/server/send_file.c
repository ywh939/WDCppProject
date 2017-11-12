#include "func.h"

void recv_file_from_client(char* name,int new_fd)
{
	int len;
	recv(new_fd,&len,sizeof(len),0);
	recv(new_fd,name,len,0);
	int fd;
	fd=open(name,O_CREAT|O_RDWR,0600);
	if(-1==fd)
	{
		perror("open");
		close(new_fd);
		return;
	}
	struct stat f_stat;
	bzero(&f_stat,sizeof(f_stat));
	fstat(fd,&f_stat);
	len=f_stat.st_size;
	lseek(fd,len,SEEK_SET);
	send(new_fd,&len,sizeof(len),0);
	int ret,total=0;
	char buf[1000]={0};
	recv(new_fd,&len,sizeof(len),0);
	while(total<len)
	{
		bzero(buf,sizeof(buf));
		ret=recv(new_fd,buf,sizeof(buf),0);
		write(fd,buf,ret);
		total=total+ret;
	}
	len=0;
	send(new_fd,&len,sizeof(len),0);
	close(fd);
}
void send_file_to_client(char* name,int new_fd)
{
	train t;
	bzero(&t,sizeof(t));
	t.len=strlen(name);
	strcpy(t.buf,name);
	send(new_fd,&t,sizeof(t.len)+t.len,0);
	int fd;
	fd=open(name,O_RDWR);
	if(-1==fd)
	{
		perror("open");
		close(new_fd);
		return;
	}
	int len;
	recv(new_fd,&len,sizeof(len),0);
	lseek(fd,len,SEEK_SET);
	struct stat f_stat;
	bzero(&f_stat,sizeof(f_stat));
	fstat(fd,&f_stat);
	bzero(&t,sizeof(t));
	t.len=f_stat.st_size-len;
	send(new_fd,&t.len,sizeof(t.len),0);
	int ret,total=0;
	while(total<t.len)
	{
		ret=sendfile(new_fd,fd,NULL,t.len-total);
		total=total+ret;
	}
	close(fd);
}
