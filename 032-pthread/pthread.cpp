#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

using namespace std;

void* thr(void* argv){
	int* i = (int*)argv;
//	while(1){
//		printf("I'm %d pthread,my pid is %d,tid is %lu\n",*i,getpid(),pthread_self());
//		sleep(1);
//	}
	//while(1);
	printf("I'm %d pthread,my pid is %d,tid is %lu\n",*i,getpid(),pthread_self());
	return (void*)i;
}

int main(){
	pthread_t tid;

	pthread_attr_t tattr;
	pthread_attr_init(&tattr);
	//pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);

	//int i;
	//for(i=0 ; i<5 ; i++){
	//	int ret = pthread_create(&tid,&tattr,thr,(void*)&i);
	//	if(ret != 0){
	//		perror("pthread_create error!\n");
	//		exit(1);
	//	}
	//	sleep(i);
	//}
	int i = 1;
	int ret = pthread_create(&tid,&tattr,thr,(void*)&i);
	if(ret != 0){
		perror("pthread_create error!\n");
		exit(1);
	}
	sleep(3);

	int c;
	pthread_join(tid,(void**)&c);
	printf("pthread_join return %d\n",c);

	pthread_attr_destroy(&tattr);

	pthread_cancel(tid);

	while(1);

	return 0;
}
