#include<iostream>
#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

using namespace std;

void sigCatch(int m){
	printf("catch you!\n");
	return;
}

int main(){
	struct sigaction act,oact;
	act.sa_handler = sigCatch;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	int ret = sigaction(SIGINT,&act,&oact);
	if(ret == -1){
		perror("sigaction error!\n");
		exit(1);
	}

	while(1);

	return 0;
}
