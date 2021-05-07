#include<iostream>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* tfun1(void*argv){
	int* num = (int*)argv;
	for(int i=0 ; i<5 ; i++){
		pthread_mutex_lock(&mutex);
		printf("the pthread1 num is %d\n",(*num)++);
		pthread_mutex_unlock(&mutex);
		usleep(20);
	}
	return NULL;
}
void* tfun2(void*argv){
	int* num = (int*)argv;
	for(int i=0 ; i<5 ; i++){
		pthread_mutex_lock(&mutex);
		printf("the pthread2 num is %d\n",(*num)++);
		pthread_mutex_unlock(&mutex);
		usleep(20);
	}
	return NULL;
}
void* tfun3(void*argv){
	int* num = (int*)argv;
	for(int i=0 ; i<5 ; i++){
		pthread_mutex_lock(&mutex);
		printf("the pthread3 num is %d\n",(*num)++);
		pthread_mutex_unlock(&mutex);
		usleep(20);
	}
	return NULL;
}

int main(){
	pthread_t tid[3];

	int i=0;
	//for(i=0 ; i<2 ; i++){
	//	int ret = pthread_create(&tid[i],NULL,tfun,(void*)&i);
	//	if(ret != 0){
	//		perror("pthread_create error!\n");
	//		exit(1);
	//	}
	//}
	pthread_create(&tid[0],NULL,tfun1,(void*)&i);
	pthread_create(&tid[1],NULL,tfun2,(void*)&i);
	pthread_create(&tid[2],NULL,tfun3,(void*)&i);
	sleep(1);

	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	pthread_mutex_destroy(&mutex);

	return 0;
}
