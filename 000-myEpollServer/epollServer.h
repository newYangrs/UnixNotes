#ifndef _EPOLLSERVER_H_
#define _EPOLLSERVER_H_
#include<string>

//初始化连接fd
int init_listen_fd(int port , int efd);

//运行epoll进行监听
void epoll_run(int port);

//监听到lfd时进行连接
void do_accept(int lfd,int efd);

//进行文件读取
void do_read(int cfd,int efd);

//获取一行内容
int get_line(int sock,char* buf,int size);

//断开连接
void disconnect(int cfd,int efd);

//服务器请求
void http_requsest(const char* ruquest,int cfd);

//发送文件
void send_file(int cfd,const char*filename);

//发送目录
void send_dir(int cfd,const char*dirname);

//编码
void encode_str(char*to,int tosize,const char*from);

//解码
void decode_str(char*to,char*from);

//获取文件属性
std::string get_file_type(const char* name);

#endif
