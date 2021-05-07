#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/un.h>
#include<sys/socket.h>
#include<string.h>

using namespace std;

#define SERVER_ADDR "server.socket"
#define CLIENT_ADDR "client.socket"

int main(){
	int cfd = socket(AF_UNIX,SOCK_STREAM,0);

	struct sockaddr_un clientAddr;
	bzero(&clientAddr,sizeof(clientAddr));
	clientAddr.sun_family = AF_UNIX;
	strcpy(clientAddr.sun_path,CLIENT_ADDR);

	int clen = offsetof(struct sockaddr_un,sun_path) + strlen(clientAddr.sun_path);

	unlink(CLIENT_ADDR);
	bind(cfd,(struct sockaddr*)&clientAddr,clen);

	struct sockaddr_un serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path,SERVER_ADDR);

	int slen = offsetof(struct sockaddr_un,sun_path) + strlen(serverAddr.sun_path);

	connect(cfd,(struct sockaddr*)&serverAddr,slen);

	char buf[BUFSIZ];
	while(fgets(buf,sizeof(buf),stdin) != NULL){
		write(cfd,buf,strlen(buf));
		int ret = read(cfd,buf,sizeof(buf));
		write(STDOUT_FILENO,buf,ret);
	}
	close(cfd);

	return 0;
}
