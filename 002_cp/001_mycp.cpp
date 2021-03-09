#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

int main(int argc,char*argv[]){
	char buf[1024];
	int n = 0;

	int fd1 = open(argv[1],O_RDONLY);
	if(fd1==-1){
		perror("open argv1 error\n");
	}
	
	int fd2 = open(argv[2],O_RDWR | O_CREAT | O_TRUNC ,0664 );
	if(fd2==-1){
		perror("open argv2 error\n");
	}

	while((n=read(fd1,buf,1024))!=0){
		if(n==-1){
			perror("read error\n");
		}
		int w = write(fd2,buf,n);
		if(w==-1){
			perror("write error\n");
		}
	}

	close(fd1);
	close(fd2);

	return 0;

}
