#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t isHave = PTHREAD_COND_INITIALIZER;
pthread_cond_t isFull = PTHREAD_COND_INITIALIZER;

struct msg{
	msg* ne;
	int num;
};

struct msg* head = NULL;
int n = 0;

void* th_consumer(void*argv){
	struct msg* mp;

	while(1){
		pthread_mutex_lock(&mutex);
		while(head == NULL){
			pthread_cond_wait(&isHave,&mutex);
		}
		mp = head;
		head = mp->ne;
		n--;

		pthread_cond_signal(&isFull);
		pthread_mutex_unlock(&mutex);

		printf("---consumer---%lu---%d---\n",pthread_self(),mp->num);
		sleep(rand()%5);
	}
	
	return NULL;
}

void* th_producter(void*argv){
	struct msg* mp;

	while(1){
		mp = (struct msg*)malloc(sizeof(struct msg));
		mp->num = rand()%1000+1;

		pthread_mutex_lock(&mutex);
		while(n == 2){
			pthread_cond_wait(&isFull,&mutex);
		}
		mp->ne = head;
		head = mp;
		n++;

		pthread_cond_signal(&isHave);
		pthread_mutex_unlock(&mutex);
		printf("producter---%lu---%d---\n",pthread_self(),mp->num);

		//pthread_cond_signal(&isHave);
		sleep(rand()%5);
	}
	
	return NULL;
}

int main(){
	srand(time(NULL));

	pthread_t cid,pid;

	pthread_create(&cid,NULL,th_consumer,NULL);
	pthread_create(&pid,NULL,th_producter,NULL);

	pthread_join(cid,NULL);
	pthread_join(pid,NULL);

	return 0;
}
