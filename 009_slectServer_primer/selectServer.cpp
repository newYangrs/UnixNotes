#include<iostream>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

using namespace std;

#define SERVER_PORT 9527


int main(){
	//创建监听fd，连接fd，最大fd
	int lsnfd,cnnfd,maxfd=0;
	//创建监听套接字
	lsnfd = socket(AF_INET,SOCK_STREAM,0);
	//设置端口复用
	int opt = 1;
	setsockopt(lsnfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	//创建并初始化服务器地址
	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//将服务器地址与监听socket绑定
	bind(lsnfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	//设置最大accept队列
	listen(lsnfd,128);
	//创建并初始化监听集合
	fd_set rset , allset;
	FD_ZERO(&allset);
	FD_SET(lsnfd,&allset);
	maxfd = lsnfd;

	while(1){
		rset = allset;		//备份监听集合

		struct timeval wtime;
		wtime.tv_sec = 0;
		wtime.tv_usec = 100;
		
		printf("everything is aready!\n");
		int ret = select(maxfd+1,&rset,NULL,NULL,NULL);		//内核监听
		cout<<ret<<endl;
		//内核发现读事件
		if(ret > 0){
			cout<<"ready to ..."<<endl;
			if(FD_ISSET(lsnfd,&rset)){
				static int connectNums = 1;
				//创建客户端地址和地址长度
				struct sockaddr_in clientAddr;
				socklen_t cAddr_len = sizeof(clientAddr);
				//创建新的连接套接字
				cnnfd = accept(lsnfd,(struct sockaddr*)&clientAddr,&cAddr_len);
				//更新最大fd
				if(maxfd < cnnfd){
					maxfd = cnnfd;
				}
				//将新创建套接字文件描述符加入监听集合
				FD_SET(cnnfd,&allset);
				//只有一个变动量，且为lsnfd
				if(ret == 1){
					continue;
				}
				printf("connectNums:%d\n",connectNums++);	
			}
			
			for(int i=lsnfd+1 ; i<=maxfd ; i++){
				if(FD_ISSET(i,&rset)){
					char buf[BUFSIZ];
					int n = read(i,buf,BUFSIZ);
					if(n == 0){
						close(i);
						FD_CLR(i,&allset);
						//continue;
					}

					printf("receive datas:\n%s\n",buf);

					for(int j=0 ; j<n ; j++){
						buf[j] = toupper(buf[j]);
					}

					printf("send datas:\n%s\n",buf);
					write(i,buf,n);
				}
			}

		}

	}

	return 0;
}


