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
	int clients[FD_SETSIZE],idx = 0;

	memset(clients,-1,sizeof(clients));

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

	static int connectNums = 1;
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
				printf("connectNums:%d\n",connectNums++);	
				//创建客户端地址和地址长度
				struct sockaddr_in clientAddr;
				socklen_t cAddr_len = sizeof(clientAddr);
				//创建新的连接套接字
				cnnfd = accept(lsnfd,(struct sockaddr*)&clientAddr,&cAddr_len);
				//更新最大fd
				if(maxfd < cnnfd){
					maxfd = cnnfd;
				}

				for(int i=0 ; i<=FD_SETSIZE ; i++){
					if(i == FD_SETSIZE){
						printf("too mamny connect!\n");
						return 0;
					}
					if(clients[i] == -1){
						clients[i] = cnnfd;
						if(idx < i){
							idx = i;
						}
						break;
					}
				}
				

				//将新创建套接字文件描述符加入监听集合
				FD_SET(cnnfd,&allset);
				//只有一个变动量，且为lsnfd
				if(ret == 1){
					continue;
				}
			}
			
			//循环寻找有读事件的socket，并进行处理
			for(int i=0 ; i<=idx ; i++){
				if(clients[i] == -1){
					continue;
				}
				if(FD_ISSET(clients[i],&rset)){
					//接收配套socket传过来的字符
					char buf[BUFSIZ];
					int n = read(clients[i],buf,BUFSIZ);
					if(n == 0){
						close(clients[i]);
						FD_CLR(clients[i],&allset);
						if(i == idx){
							clients[idx--] = -1;
						}else{
							clients[i] = -1;
						}
						continue;
					}
					printf("receive datas:\n%s\n",buf);
					//小写转大写
					for(int j=0 ; j<n ; j++){
						buf[j] = toupper(buf[j]);
					}
					//输出大写字符串，并写到配套socket
					printf("send datas:\n%s\n",buf);
					write(clients[i],buf,n);
				}
			}

		}

	}

	return 0;
}


