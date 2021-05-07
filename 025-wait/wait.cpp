#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

using namespace std;


int main(){
	pid_t pid,wpid;
	int i;

	for(i=0 ; i<5 ; i++){
		if((pid = fork()) == 0){
			break;
		}
	}
	if(i==5){
		while((wpid = waitpid(-1,NULL,WNOHANG))!=-1){
			if(wpid > 0){
				printf("wait child %d\n",wpid);
			}else{
				sleep(1);
			}
		}
	}
	if(pid == 0){
		sleep(i);
		printf("I'm NO.%d kid,ID is %d,my panrent is %d\n",i+1,getpid(),getppid());
	}
	return 0;
}
