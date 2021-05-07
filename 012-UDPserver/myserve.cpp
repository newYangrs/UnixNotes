#include<iostream>
#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

#define SEVER_PORT 9527


int main(){

	int listenfd = socket(AF_INET,SOCK_DGRAM,0);
	

	sockaddr_in severaddr;
	severaddr.sin_family = AF_INET;
	severaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	severaddr.sin_port = htons(SEVER_PORT);
	
	bind(listenfd,(struct sockaddr*)&severaddr,sizeof(severaddr));

	printf("Accepting connections ...\n");

	while(1){
		char buf[4096];

		sockaddr_in clientaddr;
		bzero(&clientaddr,sizeof(clientaddr));
		socklen_t clientaddr_len = sizeof(clientaddr);	
	
		int n = recvfrom(listenfd,buf,4096,0,(struct sockaddr*)&clientaddr,&clientaddr_len);
		if(n == -1){
			break;
		}

//		char str[INET_ADDRSTRLEN];
//		printf("receive frome &s at port %d\n",
//			inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,str,sizeof(str)),
//			ntohs(clientaddr.sin_port));

		write(STDOUT_FILENO,buf,n);

		for(int i=0 ; i<n ; i++){
			buf[i] = toupper(buf[i]);
		}

		printf("\nsend to client:\n%s\n",buf);

		sendto(listenfd,buf,n,0,(struct sockaddr*)&clientaddr,clientaddr_len);

	}
	
	close(listenfd);

	return 0;


}


