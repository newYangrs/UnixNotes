#include<iostream>
#include<stdio.h>
#include<unistd.h>

using namespace std;

int main(){
	unlink("./makefile2");
	return 0;
}
