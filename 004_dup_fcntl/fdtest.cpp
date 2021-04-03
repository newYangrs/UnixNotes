#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

using namespace std;


int main(){
	int fd1;
	fd1 = open("ftest",O_RDWR | O_CREAT | O_TRUNC ,0644);
	printf("fd1:%d\n",fd1);
	write(fd1,"12345\n",6);

	int fd2;
	fd2 = open("ftest",O_RDWR  ,0644);
	printf("fd2:%d\n",fd2);
	write(fd2,"123\n",4);

	return 0;
}

