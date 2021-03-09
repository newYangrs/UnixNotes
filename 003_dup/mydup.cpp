#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc,char*argv[]){
	int fd1=open(argv[1],O_RDWR);	
	int fd2=open(argv[2],O_RDWR);
	printf("fd2:%d\n",fd2);

	int fret1=dup2(fd1,fd2);
	printf("dup2ret:%d\n",fret1);
	write(fret1,"dup2123\n",8);

	close(fd1);
	close(fd2);

	return 0;

}
