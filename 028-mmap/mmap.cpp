#include<iostream>
#include<stdio.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<cstring>
#include<sys/wait.h>

using namespace std;

int main(){
//	int fd = open("testmmap",O_RDWR | O_CREAT | O_TRUNC,0644);
//	if(fd == -1){
//		perror("open wrong\n");
//		exit(1);
//	}
	
//	lseek(fd,0,SEEK_END);
//	write(fd,"\0",1);
//	ftruncate(fd,4);

//	int len = lseek(fd,0,SEEK_END);

	//int* p = (int*)mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	int* p = (int*)mmap(NULL,4,PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS,-1,0);
	if(p == MAP_FAILED){
		perror("mmap error\n");
		exit(1);
	}

	//strcpy(p,"hello\n");
	//printf("-----%s\n",p);
	
	pid_t pid = fork();
	if(pid > 0){
		*p = 200;
		printf("parent *p = %d\n",*p);
		waitpid(0,NULL,0);
		int ret = munmap(p,4);
		if(ret == -1){
			perror("munmap error\n");
			exit(1);
		}
	}else if(pid == 0){
		printf("child *p = %d\n",*p);
	}


	return 0;
}
