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
        std::cin >> data.buffer;
        data.type = Package::MSG;
        data.size = strlen(data.buffer);
 
        // 接收回复包
        struct Package reSend;
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
 
        int ret = -1;
 
loop_send:
        //发送信息
        sendto(sock,&data,sizeof(data),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
 
        // 判断对端是否接收到了数据包
        for(int i = 0;i < 1024;i++)
        {
            //接收回复信息
            ret = recvfrom(sock,&reSend,sizeof(reSend),MSG_DONTWAIT,(struct sockaddr*)&sender,&size);
            if(ret!=-1)break;//收到回复消息，则停止
            usleep(1000);
        }
        //未接收到回复消息success，则一直重新发送，直到接收到回复
        if(ret == -1)goto loop_send;
        if(reSend.buffer != std::string("success"))goto loop_send;
        //发送exit，关闭连接
        if(std::string(data.buffer) == "exit")break;
    }
    close(sock);
    return 0;
}