#include<iostream>
#include<sys/socket.h>
using namespace std;

int main(){
	int lsnfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	

	
	
	
	
	
	
	
	

	bind(lsnfd,(struct sockaddr*)&serverAddr,sizeof serverAddr);
}
	 

