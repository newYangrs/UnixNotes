#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include"epollServer.h"

int main(int argc,char* argv[]){
	if(argc < 3){
		printf("please cin : ....\n eg    ./a.out port path\n");
	}

	int port = atoi(argv[1]);

	int ret = chdir(argv[2]);
	if(ret == -1){
		perror("chdir error:");
		exit(1);
	}

	epoll_run(port);

	return 0;
}
