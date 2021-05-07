#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define SERVER_PORT 9527

using namespace std;



int main(){
	int connfd = socket(AF_INET,SOCK_DGRAM,0);
	
	sockaddr_in serveaddr;
	bzero(&serveaddr,sizeof(serveaddr));
	serveaddr.sin_family = AF_INET;
	serveaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,"192.168.20.190",&serveaddr.sin_addr.s_addr);

	bind(connfd,(struct sockaddr*)&serveaddr,sizeof(serveaddr));
	

	int m = 5;
	char buf[1024];
	while(m--){
		sendto(connfd,"hello\n",7,0,(struct sockaddr*)&serveaddr,sizeof(serveaddr));
		int n = recvfrom(connfd,buf,1024,0,NULL,NULL);
		write(STDOUT_FILENO,buf,n);
		sleep(2);
	}

	close(connfd);
	
	return 0;
}
