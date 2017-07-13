#include  "define_header_file.h"
//定义客户端地址信息
typedef struct  client_info
{
    char client_id[10];  
    char msg[10];
    char name[30];
    char password[30];
    int res;
    struct sockaddr_in client_addr;
    int   client_addr_len;  
}client_info;
//定义通信结构信息
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

void *  send_info(void*arg)
{
    thread_block  thread_bl=*(thread_block*)arg;
    communication_info  c_info;
    while(1)
    {
        gets(c_info.dest_client_id);
        gets(c_info.msg);
        sendto(thread_bl.fd,(void*)&c_info,sizeof(c_info),0,(struct sockaddr*)&thread_bl.addr,thread_bl.addr_len);
        //send(thread_bl.fd,&c_info,sizeof(c_info),0);
    }   
}
void *  recv_info(void*arg)
{
//     thread_block  thread_bl=*(thread_block*)arg;
//     communication_info  c_info;
//     while(1)
//     {
//         bzero(c_info.msg,sizeof(c_info.msg));
//         //recvfrom(thread_bl.fd,&c_info,sizeof(c_info),0,NULL,NULL);
//         //recvfrom(thread_bl.fd,&c_info,sizeof(c_info),0,(struct sockaddr*)&thread_bl.addr,thread_bl.addr_len);
//         //recv(thread_bl.fd,&c_info,sizeof(c_info)-1,0);
//         printf("recv %s send %s\n",c_info.dest_client_id,c_info.msg);
//     }
}

int main(int argc,const char* argv[])
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
    server_addr.sin_port=htons(8888);
    server_addr.sin_addr.s_addr=inet_addr("47.95.9.185");
    //server_addr.sin_addr.s_addr=INADDR_ANY;
    if(connect(client_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==0)
    {
        //输入客户端信息
        client_info   c_info;
        printf("user or led:");
        gets(c_info.client_id);
        send(client_fd,c_info.client_id,strlen(c_info.client_id),0);
        if(strcmp(c_info.client_id,"user")==0)
        {
            printf("new or old:");
            gets(c_info.msg);
            send(client_fd,c_info.msg,strlen(c_info.msg),0);
            if(strcmp(c_info.msg,"new")==0)
            {
                printf("username:");
                gets(c_info.name);
                send(client_fd,c_info.name,strlen(c_info.name),0);
                printf("password:");
                gets(c_info.password);
                send(client_fd,c_info.password,strlen(c_info.password),0);
                exit(-1);
            }
            else if(strcmp(c_info.msg,"old")==0)
            {
                printf("username:");
                gets(c_info.name);
                send(client_fd,c_info.name,strlen(c_info.name),0);
                printf("password:");
                gets(c_info.password);
                send(client_fd,c_info.password,strlen(c_info.password),0);
                recv(client_fd,&c_info.res,4,0);
                if(c_info.res==1)
                {
                    printf("success\n");
                }
                else
                {
                    printf("error\n");
                    exit(-1);
                }
            }
            else
            {
                exit(-1);
            }
        }
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
            //printf("udp_fd=%d\n", cli_udp_fd);
            c_info.client_addr.sin_addr.s_addr=inet_addr("inet_ntoa(c_info.client_addr.sin_addr)");
            if(bind(cli_udp_fd,(struct sockaddr*)&c_info.client_addr,c_info.client_addr_len)==-1)
            {
                perror("udp bind  error:");
                exit(-1);
            }
            thread_block  thread_bl;
            thread_bl.fd=cli_udp_fd;
            thread_bl.addr=server_addr;
            thread_bl.addr_len=sizeof(server_addr);
            //线程实现消息收发   
            pthread_t  pid1,pid2;
            int r=pthread_create(&pid1,NULL,send_info,&thread_bl);
            while(r==-1)
            {
                r=pthread_create(&pid1,NULL,send_info,&thread_bl);
            }    
            r=pthread_create(&pid2,NULL,recv_info,&thread_bl);
            while(r==-1)
            {
                r=pthread_create(&pid2,NULL,recv_info,&thread_bl);
            }      
            pthread_join(pid1,NULL);
            pthread_join(pid2,NULL);
        }        
    }
    else 
    {
        perror("connect :");
        exit(-1);
    }
    return 0;
}