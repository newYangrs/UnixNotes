
#include<sys/epoll.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<string>
#include"epollServer.h"


//运行epoll进行监听
void epoll_run(int port){
	printf("epoll is running!\n");

	return ;
}

//初始化连接fd
int init_listen_fd(int port , int efd){
	int lfd = socket(AF_INET,SOCK_STREAM,0);
	if(lfd == -1){
		perror("socket error:");
		exit(1);
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int flag = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));

	int ret = bind(lfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(ret == -1){
		perror("bind error:");
		exit(1);
	}

	ret = listen(lfd,64);
	if(ret == -1){
		perror("listen error:");
		exit(1);
	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	ret = epoll_ctl(efd,EPOLL_CTL_ADD,lfd,&ev);
	if(ret == -1){
		perror("epoll_ctl error:");
		exit(1);
	}

	return lfd;
}

//监听到lfd时进行连接
void do_accept(int lfd,int efd);

//进行文件读取
void do_read(int cfd,int efd);

//断开连接
void disconnect(int cfd,int efd);

//服务器请求
void http_requsest(const char* ruquest,int cfd);

//发送文件
void send_file(int cfd,const char*filename);

//发送目录内容
void send_dir(int cfd,const char*dirname){
	int i,ret;

	char buf[4096] = {0};
	sprintf();
}

//根据文件名获取文件属性
std::string get_file_type(const char* name){
	std::string dot;

	dot = strrchr(name,'.');
	if(dot.empty()){
		return "text/palin; charset=utf-8";
	}else if(dot == ".html" || dot == "htm"){
		return "text:html; charset=utf-8";
	}else if(dot == ".jpg" || dot == "jpeg"){
		return "image/jpeg";
	}else if(dot == "avi"){
		return "video/x-msvideo";
	}else if(dot == ".mp3"){
		return "audio/mpeg";
	}else if(dot == ".png"){
		return "image/png";
	}

	return "text/plain; charset=utf-8";
}

//解析http请求消息的每一行内容
int get_line(int sock,char* buf,int size){
	int i = 0;
	char c = '\0';
	int n;
	while((i < size - 1)&&(c != '\n')){
		n = recv(sock,&c,1,0);
		if(n>0){
			if(c == '\r'){
				n = recv(sock,&c,1,MSG_PEEK);
				if((n > 0)&&(c == '\n')){
					recv(sock,&c,1,0);
				}else{
					c = '\n';
				}
			}
			buf[i] = c;
			i++;
		}else{
			c = '\n';
		}
	}
	buf[i] = '\0';
	
	return i;
}

//将16进制数转化为10进制数
int hexit(char c){
	if(c >= '0' && c <= '9'){
		return c-'0';
	}
	if(c >= 'a' && c <= 'f'){
		return c-'a'+10;
	}
	if(c >= 'A' && c <= 'F'){
		return c-'A'+10;
	}

	return 0;
}

//编码
void encode_str(char*to,int tosize,const char*from){
	int tolen;

	for (tolen = 0; *from != '\0' && tolen + 4 < tosize; ++from) {

		if (isalnum(*from) || strchr("/_.-~", *from) != (char*)0) {
			*to = *from;
			++to;
			++tolen;
		} else {
			sprintf(to, "%%%02x", (int) *from & 0xff);
			to += 3;
			tolen += 3;
		}
	}
	*to = '\0';
}

//解码
void decode_str(char*to,char*from){
	for ( ; *from != '\0'; ++to, ++from  ) {
		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2])) {
			*to = hexit(from[1])*16 + hexit(from[2]);
			from += 2;                      
		} else {
			*to = *from;
		}
	}
	*to = '\0';
}

