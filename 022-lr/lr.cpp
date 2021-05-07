#include<iostream>
#include<stdio.h>
#include<cstring>
#include<dirent.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>

using namespace std;

void isFile(const char* name);

void read_dir(const char* dir){
	char path[1024];
	DIR* dp;
	struct dirent* dnty;



	dp = opendir(dir);
	if(dp == NULL){
		perror("opendir error!\n");
		return ;
	}

	while((dnty = readdir(dp)) != NULL){ 
		if((strcmp(dnty->d_name,".")==0) || (strcmp(dnty->d_name,"..")==0)){
			continue;
		}
		sprintf(path,"%s/%s",dir,dnty->d_name);
		isFile(path);
	}
	
	closedir(dp);
	return;
}

void isFile(const char* name){
	struct stat sd;

	int ret = stat(name,&sd);
	if(ret == -1){
		perror("stat error!\n");
		exit(1);
	}

	if(S_ISDIR(sd.st_mode)){
		read_dir(name);
	}else{
	printf("%10s\t\t%ld\n",name,sd.st_size);

	}
	
	return;
}

int main(int argc,char* argv[]){
	if(argc == 1){
		isFile(".");
	}else{
		isFile(argv[1]);
	}
}

