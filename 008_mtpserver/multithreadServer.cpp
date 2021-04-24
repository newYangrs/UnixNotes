#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>

#define SERVER_PORT 9527

using namespace std;

struct t_node{
	 int cntfd;
	 struct sockaddr_in clientaddr;
};

void* do_toupper(void*arg){
	char buf[1024];
	struct t_node* t_ar = (struct t_node*)arg;
	
	pthread_detach(pthread_self());

	while(1){
		int n = read(t_ar->cntfd,buf,1024); 
		if(n == 0){
			 break;
		}

		printf("pthread %lu receive: %s\n",pthread_self(),buf);

		for(int j = 0 ; j<n ; j++){
			 buf[j] = toupper(buf[j]);
		}
		printf("send:    %s\n",buf);
		write(t_ar->cntfd,buf,n);

	}
	close(t_ar->cntfd);
	pthread_exit((void*)1);
	 
}


int main(){
	struct t_node t[1024];
	static int i = 1;
	

	int lsnfd = socket(AF_INET,SOCK_STREAM,0);
	

	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(lsnfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));

	listen(lsnfd,20);
	
	printf("-------------------------------\n\n"
			"Server is ready for connecting\n\n"
			"------------------------------\n");

	
	while(1){
		struct sockaddr_in clientaddr;
		socklen_t saddrlen = sizeof(serveraddr);

		int cfd = accept(lsnfd,(struct sockaddr*)&serveraddr,&saddrlen);

		t[i].clientaddr = clientaddr;
		t[i].cntfd = cfd;
		
		pthread_t tid;
		pthread_create(&tid,NULL,do_toupper,(void*)&t[i]);
		i++;
	}

	return 0;
}
