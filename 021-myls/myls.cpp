#include<iostream>
#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<cstring>

using namespace std;

int main(int argc,char* argv[]){
	DIR* dp;
	struct dirent* de;

	if(argc>1){
		dp = opendir(argv[1]);
		if(dp == NULL){
			perror("opendir error!\n");
			exit(1);
		}
	}else{
		dp = opendir(".");
		if(dp == NULL){
			perror("opendir error!\n");
			exit(1);
		}
	}

	while((de = readdir(dp)) != NULL){
		if((strcmp(de->d_name,".")==0)||(strcmp(de->d_name,"..")==0)){
			continue;
		}
		printf("%s\t",de->d_name);
	}
	printf("\n");

	closedir(dp);
	return 0;
}
