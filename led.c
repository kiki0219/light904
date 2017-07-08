#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include  "define_header_file.h"

//定义客户端地址信息
typedef struct  client_info
{
    char client_id[10];  
    struct sockaddr_in client_addr;
    int   client_addr_len;  
}client_info;
//定于通信结构信息
typedef  struct   communication_info
{
    char dest_client_id[10];  
    char  msg[100];
} communication_info;

//定义线程参数结构
typedef struct  thread_block
{
    int  fd;
    struct sockaddr_in  addr;
    socklen_t  addr_len;
}thread_block;

// void *  send_info(void*arg)
int main(int argc,const char **argv)
{

	int client_fd=socket(AF_INET,SOCK_STREAM,0);
    if(client_fd==-1)
    {
        perror("socket  init fail\n");
        exit(-1);
    }
    //初始化地址结构=地址族+地址值+端口值
    struct  sockaddr_in  server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(1234);
    //listen_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr=inet_addr("192.168.1.100");
    if(connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==0)
    {
        //输入客户端信息
        client_info   c_info;
        printf("please input id:");
        gets(c_info.client_id);
        send(client_fd,c_info.client_id,strlen(c_info.client_id),0);
        int ret=recv(client_fd,&c_info.client_addr,sizeof(c_info.client_addr),0);
        if(ret>0)
        {
            printf("%s\t%d\n",inet_ntoa(c_info.client_addr.sin_addr),ntohs(c_info.client_addr.sin_port));
            c_info.client_addr_len=sizeof(c_info.client_addr);
            int cli_udp_fd=socket(AF_INET,SOCK_DGRAM,0);
            if(client_fd==-1)
            {
                perror("socket udp error:");
                exit(-1);
            }
            if(bind(cli_udp_fd,(struct sockaddr*)&c_info.client_addr,c_info.client_addr_len)==-1)
            {
                perror("udp bind  error:");
                exit(-1);
            }
            thread_block  thread_bl;
            thread_bl.fd=cli_udp_fd;
            thread_bl.addr=server_addr;
            thread_bl.addr_len=sizeof(server_addr);
            
		    communication_info  c_info;
		    int fd=open("dev/led",O_WRONLY);
			if(fd<0)
			{
				printf("devices open faild\n");
			}
			int num=1;
			write(fd,&num,1);
		    while(1)
		    {
		    	//通信
		        bzero(c_info.msg,sizeof(c_info.msg));
		        recvfrom(thread_bl.fd,c_info.msg,sizeof(c_info.msg),0,NULL,NULL);
                //gpio
				num=atoi(c_info.msg);
				printf("%d\n",num );//显示输入值
				
				int ret=write(fd,&num,1);
				
		    }
		    close(fd);
        }  
    }
    else 
    {
        perror("connect :");
        exit(-1);
    }
	
	return 0;
}
