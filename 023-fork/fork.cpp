#include<iostream>
#include<stdio.h>
#include<unistd.h>

using namespace std;


int main(){
	pid_t pid;
	int i;

	for(i=0 ; i<5 ; i++){
		if((pid = fork()) == 0){
			break;
		}
	}
	if(i==5){
		sleep(5);
		printf("I'm panrent %d\n",getpid());
	}
	if(pid == 0){
		sleep(i);
		printf("I'm NO.%d kid,ID is %d,my panrent is %d\n",i+1,getpid(),getppid());
	}
	return 0;
}
