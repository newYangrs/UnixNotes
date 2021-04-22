#include<iostream>
#include<stdio.h>
#include<sys/socket.h>

using namespace std;




int main(){
	int lsnfd = socket(AF_INET,SOCK_STREAM,0);

	bind(lsnfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

	listen(lsnfd,20);

	pthread_create();
}
