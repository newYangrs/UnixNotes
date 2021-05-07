#include<iostream>
#include<stdio.h>
#include<unistd.h>

using namespace std;

int main(){
	int i = 1;
	pid_t pid;

	pid = fork();
	if(pid == 0){
		execlp("ls","ls","-a",NULL);
		printf("The 'i' is %d\n",i);
	}else{
		printf("My son is %d,my ID is %d\n",pid,getpid());
	}

	return 0;

}
