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
    local.sin_addr.s_addr= INADDR_ANY;
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

    int sock = createSocket(argv[1],atoi(argv[2]));
    if(sock < 0)
    {
        perror("Error");
        return -1;
    } 
 
     // 目标地址
    struct sockaddr_in dest_addr;
    dest_addr.sin_family     = AF_INET;
    dest_addr.sin_port       = htons(atoi(argv[2]));
    dest_addr.sin_addr.s_addr= inet_addr(argv[1]);
    while(1)
    {
        struct Package data;
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
 
        //接收信息
        recvfrom(sock,&data,sizeof(data),0,(struct sockaddr*)&sender,&size);
 
        // 回复对端一个数据，表示我接收到了你的数据
        struct Package reSend;
        reSend.num  = data.num;
        reSend.type = Package::MSG;
        strcpy(reSend.buffer,"success");//将success作为回复的消息发送给发送端
        sendto(sock,&reSend,sizeof(reSend),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        
        //收到exit则断开连接
        if(std::string(data.buffer) == "exit")break;
        std::cout << "来自["<< inet_ntoa(sender.sin_addr) <<"]的消息：" << data.buffer<< std::endl;
    }
    close(sock);
    return 0;
}