/*************************************************************************
	> File Name: recvfile.c
	> Author:clj
	> Mail: 842441774@qq.com 
	> Created Time: 2013年12月15日 星期日 19时23分13秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<string.h>
#include<netinet/in.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>


#define MAX 1024
void sig_chld(int);
void copyfile(int,FILE *);
int recvf(char * port,char *filename)
{
	FILE *fp;
	pid_t childpid;
	struct sigaction act;
	act.sa_handler=sig_chld;
	act.sa_flags=0;

	sigaction(SIGCHLD,&act,NULL);

	if((fp=fopen(filename,"ab+"))==NULL)
	{
		printf("file open error!");
	    return -1; 
	}
	int listenfd ,connfd;
	struct sockaddr_in recvaddr,sendaddr;
	int  newport,sendlen;
	newport=atoi(port);
    printf("the communication port is %d\n",newport);

	if((listenfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
			{
				printf("socket error\n");
				return -1;
			}
	printf("the listening port is %d\n",listenfd);

	bzero(&recvaddr,sizeof(recvaddr));
	recvaddr.sin_family=AF_INET;
	recvaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	recvaddr.sin_port=htons(newport);

	if(bind(listenfd,(const struct sockaddr *)&recvaddr,sizeof(recvaddr)) < 0)
	{
		printf("bind error\n");
		return -1;
	} 
	printf("binding success\n");

	if(listen(listenfd,5) < 0)
		{
			printf("listen error\n");
			return -1;
		}

	printf("listeing success!\n");

	while(1)
	{
		sendlen=sizeof(sendaddr);
		if((connfd=accept(listenfd,(struct sockaddr*) &sendaddr,&sendlen)) < 0)
		{
			printf("accept error\n");
			printf("accept error number is %d\n",connfd);
			break;
			return -1;
		}
		printf("connection fd is %d\n",connfd);
		printf("accept success\n");

		if ((childpid=fork()) ==0)
		{
			close(listenfd);
			copyfile(connfd,fp);
			close(fp);
			printf("receing complete!\n");
		}
		close(connfd);
	}

	return 0;
}

void copyfile(int sockfd,FILE *fp)
{
	ssize_t n;
	char buf[MAX];
	while((n=read(sockfd,buf,MAX)) > 0)
	{
		printf("the file is receiving\n");
		fwrite(buf,1,n,fp);
	}

	if(n == 0)
	{
		printf("normally exit\n");
	}

	if(n < 0 )
		printf("read error\n");
}

int main(int argc,char**argv)
{
	if (argc < 3)
	{
		printf("please offer a filename\n");
		return -1;
	}

	if (argc <2)
	{
		printf("please offer a port number\n");
		return -1;
	}

	int stat;

	stat=recvf(argv[1],argv[2]);
	if (stat == -1)
		printf("run happends error\n");
	else 
		printf("run good\n");

	return 0;
 
}
void sig_chld(int signo)
{
	pid_t pid ;
	int stat;
	while (( pid = waitpid(-1,&stat,WNOHANG)) > 0)
		printf("child %d terminaated\n",pid);

	return ;
}
