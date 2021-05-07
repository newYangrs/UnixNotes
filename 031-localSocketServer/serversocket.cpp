#include<iostream>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/un.h>

using namespace std;

#define SERVER_ADDR "server.socket"

int main(){
	int lfd = socket(AF_UNIX,SOCK_STREAM,0);

	struct sockaddr_un serverAddr;
	serverAddr.sun_family = AF_UNIX;
	strcpy(serverAddr.sun_path,SERVER_ADDR);

	int slen = offsetof(struct sockaddr_un,sun_path) + strlen(serverAddr.sun_path);

	unlink(SERVER_ADDR);
	bind(lfd,(struct sockaddr*)&serverAddr,slen);

	listen(lfd,20);

	printf("wait for connecting...\n");

	while(1){
		struct sockaddr_un clientAddr;
		int clen = sizeof(clientAddr);

		int cfd = accept(lfd,(struct sockaddr*)&clientAddr,(socklen_t*)&clen);

		clen -= offsetof(struct sockaddr_un,sun_path);
		clientAddr.sun_path[clen] = '\0';

		printf("client bind filename is : %s\n",clientAddr.sun_path);

		int size;
		char buf[BUFSIZ];
		while((size = read(cfd,buf,sizeof(buf)))>0){
			for(int i=0 ; i<size ; i++){
				buf[i] = toupper(buf[i]);
			}
			write(cfd,buf,size);
		}
		close(cfd);
	}
	close(lfd);

	return 0;
}
