#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>

using namespace std;

pthread_rwlock_t rwlock;

void* th_write(void*argv){
	int* n = (int*)argv;
	for(int i=0 ; i<10 ; i++){
		pthread_rwlock_wrlock(&rwlock);

		printf("this is pthread---%lu----write n=%d\n",pthread_self(),(*n)++);

		pthread_rwlock_unlock(&rwlock);
		usleep(500);
	}
	return NULL;
}

void* th_read(void*argv){
	int* n = (int*)argv;
	for(int i=0 ; i<10 ; i++){
		pthread_rwlock_rdlock(&rwlock);

		printf("this is pthread---%lu---read n=%d\n",pthread_self(),*n);

		pthread_rwlock_unlock(&rwlock);
		usleep(500);
	}
	return NULL;
}

int main(){
	pthread_t tid[8];

	pthread_rwlock_init(&rwlock,NULL);

	int n=0;
	for(int i=0 ; i<3 ; i++){
		pthread_create(&tid[i],NULL,th_write,(void*)&n);
	}

	for(int i=0 ; i<5 ; i++){
		pthread_create(&tid[i+3],NULL,th_read,(void*)&n);
	}

	for(int i=0 ; i<8 ; i++){
		pthread_join(tid[i],NULL);
	}

	pthread_rwlock_destroy(&rwlock);

	return 0;
}
