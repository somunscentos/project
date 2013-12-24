/*************************************************************************
	> File Name: sendf.c
	> Author:clj
	> Mail:842441774@qq.com 
	> Created Time: 2013年12月15日 星期日 20时44分51秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>

#define MAX 1024 

int sendf(char* serverip,char* port,char *filename)
{
	FILE *fp;
	if((fp=fopen(filename,"rb")) == NULL)
	{
		printf("file open error\n");
		return -1;
	}
	printf("open file success\n");

	struct sockaddr_in  serveraddr ;
	bzero(&serveraddr,sizeof(serveraddr));
	int sockfd;
	int newport;
	char buf[MAX];
	newport=atoi(port);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(newport);
	
	if (inet_aton(serverip,&serveraddr.sin_addr) == 0)
	{
		printf("tansfer ip address error\n");

	}

	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
			{
				printf("socket error\n");
				return -1;
			
			}
	printf("the sockfd is %d\n",sockfd);

	if((connect(sockfd,(const struct sockaddr *)&serveraddr,sizeof(serveraddr))) < 0)
	{
		printf("connect error\n");
		return -1;
	}
	printf("connect success!\n");

	ssize_t n;
	while((n=fread(buf,1,MAX,fp)) > 0)
		{ 
			printf("start read soucre file\n");
		//	write(sockfd,buf,n);
		   send(sockfd,buf,n,0);
		}

	if (n == 0)
		printf("exit normally\n");

	if (n < 0)
		printf("fread error");
	
	return 0;

}

int main(int argc,char **argv)
{
<<<<<<< HEAD
=======
	printf("welcome to the filestranser system\n");
>>>>>>> experimental
	if (argc < 2)
	{
		printf("please offer a ip address\n");
		return -1;
	}

	if (argc <3)
	{
		printf("please offer a port number\n");
		return -1;
	}

	if (sendf(argv[1],argv[2],argv[3]) < 0)
	{
		printf("sendf error\n");
		return -1;
	}

	if (argc < 4)
	{
		printf("please offer a filename\n");
		return -1;
	}

	return 0;
}
