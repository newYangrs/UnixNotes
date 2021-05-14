#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>

using namespace std;

#define SERVER_PORT 9527
#define MAXLINE 20


int main(){
	int lfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serverAddr;

	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVER_PORT);

	bind(lfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

	listen(lfd,20);

	struct epoll_event tmp , rfds[MAXLINE];

	tmp.data.fd = lfd;
	tmp.events = EPOLLIN;

	int efd = epoll_create(20);

	epoll_ctl(efd,EPOLL_CTL_ADD,lfd,&tmp);

	while(1){
		int nready = epoll_wait(efd,rfds,MAXLINE,1);
		if(nready == -1){
			perror("epoll_wait error!\n");
			exit(1);
		}
		for(int i=0 ; i<nready ; i++){
			if(rfds[i].events != EPOLLIN){
				continue;
			}
			
			if(rfds[i].data.fd == lfd){
				struct sockaddr_in clientAddr;
				bzero(&clientAddr,sizeof(clientAddr));
				socklen_t clientAddr_len = sizeof(serverAddr);

				int cfd = accept(lfd,(struct sockaddr*)&clientAddr,&clientAddr_len);

			//	int cst = fcntl(cfd,F_GETFL);
			//	cst |= O_NONBLOCK;
			//	fcntl(cfd,F_SETFL,cst);

				tmp.data.fd = cfd;
				tmp.events = EPOLLIN | EPOLLET;
				//tmp.events = EPOLLIN;

				epoll_ctl(efd,EPOLL_CTL_ADD,cfd,&tmp);

				if(--nready == 0){
					break;
				}
			}else{
				char buf[10];
				int n = read(rfds[i].data.fd,buf,10);
				if(n == 0){
					close(rfds[i].data.fd);
					epoll_ctl(efd,EPOLL_CTL_DEL,rfds[i].data.fd,NULL);
					printf("del %d \n",rfds[i].data.fd);
					break;
				}


				printf("receive buf:%s\n",buf);

				for(int i=0 ; i<n ; i++){
					buf[i] = toupper(buf[i]);
				}

				printf("--------------send buf:%s\n",buf);

				write(rfds[i].data.fd,buf,n);

				if(--nready == 0){
					break;
				}
				
			}
		}
	}

	close(lfd);

	return 0;
}
