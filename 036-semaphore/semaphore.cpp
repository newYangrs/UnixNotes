#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

using namespace std;

#define NUM 5

int queue[5];
sem_t blank_num,product_num;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int i = 0 , j = 0;
void* th_producer(void*argv){

	while(1){
		sem_wait(&blank_num);

		pthread_mutex_lock(&mutex);
		queue[i] = rand()%1000+1;
		printf("producer---%lu---%d---%d---\n",pthread_self(),i,queue[i]);
		i = (i+1)%5;
		pthread_mutex_unlock(&mutex);
		sem_post(&product_num);

		sleep(rand()%3);
	}

	return NULL;
}

void* th_consumer(void*argv){

	while(1){
		sem_wait(&product_num);

		pthread_mutex_lock(&mutex);
		printf("consumer---%lu---%d---%d---\n",pthread_self(),j,queue[j]);
		queue[i] = 0;
		j = (j+1)%5;
		pthread_mutex_unlock(&mutex);
		sem_post(&blank_num);

		sleep(rand()%3);
	}

	return NULL;
}

int main(){
	srand(time(NULL));

	sem_init(&blank_num,0,NUM);
	sem_init(&product_num,0,0);

	pthread_t pid[2],cid[2];

	for(int i=0 ; i<2 ; i++){
		pthread_create(&pid[i],NULL,th_producer,NULL);
		pthread_create(&cid[i],NULL,th_consumer,NULL);
	}

	for(int i=0 ; i<2 ; i++){
		pthread_join(pid[i],NULL);
		pthread_join(cid[i],NULL);
	}

	sem_destroy(&blank_num);
	sem_destroy(&product_num);

	return 0;
}
