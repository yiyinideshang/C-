#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
  
struct Package
{
    int num;
    enum{MSG,CMD,IMG} type;
    int size;
    char buffer[2048];
};
 
int createSocket(std::string ipv4,int port)
{
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock == -1)return -1;
    // 绑定本地地址
    struct sockaddr_in local;
    local.sin_family     = AF_INET;
    local.sin_port       = htons(port);
    local.sin_addr.s_addr= inet_addr(ipv4.c_str());
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) == -1)return -2;
    return sock;
}
int main(int argc,const char *argv[])
{
    if(argc != 3) 
    {
        std::cout << "用法: " << argv[0] << " <IP地址> <端口号>" << std::endl;
        return -1;
    }
    std::cout<<"正在等待接受数据..."<<std::endl;

    int recv_sock = createSocket(argv[1],atoi(argv[2]));
    if(recv_sock < 0)
    {
        perror("Error");
        return -1;
    } 
    while(1)
    {
        struct Package data;
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
 
        recvfrom(recv_sock,&data,sizeof(data),0,(struct sockaddr*)&sender,&size);
 
        if(std::string(data.buffer) == "exit")break;
        std::cout << "来自["<< inet_ntoa(sender.sin_addr) <<"]的消息：" << data.buffer<< std::endl;
    }
    close(recv_sock);
    return 0;
}