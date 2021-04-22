#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define SERVER_PORT 9527

using namespace std;



int main(){
	int connfd = socket(AF_INET,SOCK_STREAM,0);
	
	sockaddr_in serveaddr;
	serveaddr.sin_family = AF_INET;
	serveaddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,"192.168.20.203",&serveaddr.sin_addr.s_addr);
	
	connect(connfd,(struct sockaddr*)&serveaddr,sizeof(serveaddr));

	int m = 5;
	while(m--){
		write(connfd,"hello\n",7);
		char buf[1024];
		int n = read(connfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,n);
		sleep(2);
	}

	close(connfd);
	
	return 0;
}
