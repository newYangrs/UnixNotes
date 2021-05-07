#include<iostream>
#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

using namespace std;

void print_set(sigset_t* set){
	for(int i=1 ; i<32 ; i++){
		if(sigismember(set,i)){
			putchar('1');
		}else{
			putchar('0');
		}
	}
	printf("\n");
}

int main(){
	sigset_t set , pset , oset;

	sigemptyset(&set);
	sigaddset(&set,SIGINT);
	sigaddset(&set,SIGBUS);
	sigaddset(&set,SIGSTOP);
	sigaddset(&set,SIGKILL);

	int ret = sigprocmask(SIG_BLOCK,&set,&oset);
	if(ret == -1){
		perror("sigprocmask error!\n");
		exit(1);
	}

	while(1){
		int ret = sigpending(&pset);
		if(ret == -1){
			perror("sigprocmask error!\n");
			exit(1);
		}

		print_set(&pset);
		sleep(1);
	}
}
