#include<fcntl.h>
#include<unistd.h>

int main(){
	int fd1 = open("test",O_RDWR | O_CREAT | O_TRUNC ,0644);
	write(fd1,"test1\n",6);

	int fd2;
	fd2 =fcntl(fd1,F_DUPFD,0);
	write(fd2,"test2\n",6);

	int fd3;
	fd3 = fcntl(fd2,F_DUPFD,fd3);
	write(fd3,"test3\n",6);
	
	close(fd1);
	close(fd2);
	close(fd3);

	return 0;
}
