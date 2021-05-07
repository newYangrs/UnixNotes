#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>

void do_sigchild(int num){
	pid_t wpid;
	while((wpid = waitpid(0,NULL,WNOHANG)) != -1){
		if(wpid >0 ){
			printf("waitpid is %d\n",wpid);
		}	
	}
	//wait(NULL);
}

int main(){
	struct sigaction act;
	act.sa_handler = do_sigchild;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	struct sockaddr_in clientAddr;
	socklen_t clientAddr_len;

	sigaction(SIGCHLD,&act,NULL);

	int lsnfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in serverAddr;

	bzero(&serverAddr,sizeof (serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9527);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(lsnfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

	listen(lsnfd,20);

	pid_t pid;
	int cntfd;

	while(1){
		bzero(&clientAddr,sizeof(clientAddr));
		clientAddr_len = sizeof(clientAddr);

		printf("\nclientAddr = \n%u\n\n",clientAddr_len);
		cntfd = accept(lsnfd,(struct sockaddr*)&clientAddr,&clientAddr_len);

		pid = fork();

		static int forkNum = 1;
		printf("new fork%d\n",forkNum++);

		if(pid == 0){
			printf("new fork%d\n",forkNum++);
			close(lsnfd);	
			break;

		}else{
			static int parentcnt = 1;
			printf("parent%d\n",parentcnt++);
			close(cntfd);
		}
	}
	if(pid == 0){
		printf("Thread%d aready connect,wait for client's data:\n",getpid());
		while(1){
			static int cnt = 1;
			char buf[4096];
			int n = read(cntfd,buf,4096);
			if(n <= 0){
				break;
			}

			printf("client send %d:\n",cnt);
			write(STDOUT_FILENO,buf,n);

			for(int i=0 ; i<n ; i++){
				buf[i] = toupper(buf[i]);
			}

			printf("send to client %d:\n",cnt);
			write(STDOUT_FILENO,buf,n);

			write(cntfd,buf,n);
			sleep(1);
			cnt++;
		}
		close(cntfd);
		close(5);
		return 0;
	}

	return 0;
}

