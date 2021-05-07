#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

using namespace std;


//配置哲学家i左边和右边的宏
#define LEFT (i+5-1)%5
#define RIGHT (i+1)%5

//设置哲学家状态
#define thinking 0
#define hungry 1
#define eating 2

//存储每个哲学家的状态
int philosopher_state[5];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;		//创建一把锁
sem_t philosopher_sem[5];								//为每一个哲学家创建一个信号量，如果没拿到叉子便阻塞，需要初始化为0

//哲学家思考函数
void do_thinking(int i){
	if(philosopher_state[i] == thinking){
		printf("philosopher-%d is thinking---\n",i+1);
		sleep(2);
	}

	return ;
}

//哲学家吃饭
void do_eating(int i){
	if(philosopher_state[i] == eating){
		printf("philosopher-%d is eating---\n",i+1);
		sleep(3);
	}
	return ;
}

//哲学家i检查左右邻居是否在吃饭，仅当左右邻居都没吃饭时，哲学家i才可以吃饭
void check_neighbor_state(int i){
	if(philosopher_state[i]==hungry && philosopher_state[LEFT]!=eating && philosopher_state[RIGHT]!=eating){
		philosopher_state[i] = eating;
		sem_post(&philosopher_sem[i]);
	}
	
	return ;
}

//哲学家i准备吃饭，尝试同时拿起两把叉子，如果失败，则阻塞，等待左、右邻居唤醒，重新尝试拿起两把叉子
void take_fork(int i){
	pthread_mutex_lock(&mutex);
	philosopher_state[i] = hungry;
	check_neighbor_state(i);
	pthread_mutex_unlock(&mutex);
	sem_wait(&philosopher_sem[i]);
	printf("philosopher-%d take forks----\n",i+1);

	return ;
}

//哲学家i放下叉子，同时唤醒左右邻居，让他们尝试拿起两把叉子吃饭
void put_fork(int i){
	pthread_mutex_lock(&mutex);
	philosopher_state[i] = thinking;
	check_neighbor_state(LEFT);
	check_neighbor_state(RIGHT);
	printf("philosopher-%d put forks---\n",i+1);
	pthread_mutex_unlock(&mutex);

	return ;
}

//每个哲学家的动作循环
void* philosopher(void*argv){
	int i = *(int*)argv;
	while(1){
		do_thinking(i);
		take_fork(i);
		do_eating(i);
		put_fork(i);
	}
	return NULL;
}

int main(){
	//初始化每个哲学家的信号量为0，代表没拿起两把叉子
	for(int i=0 ; i<5 ; i++){
		sem_init(&philosopher_sem[i],0,0);
	}

	//创建五个哲学家线程
	pthread_t tid[5];
	int i;
	int id[5];
	for(i=0 ; i<5 ; i++){
		id[i] = i;
		pthread_create(&tid[i],NULL,philosopher,(void*)&id[i]);
	}

	//回收五个哲学家线程
	for(i=0 ; i<5 ; i++){
		pthread_join(tid[i],NULL);
	}

	//销毁每个哲学家的信号量
	for(i=0 ; i<5 ; i++){
		sem_destroy(&philosopher_sem[i]);
	}

	pthread_mutex_destroy(&mutex);

	return 0;
}
