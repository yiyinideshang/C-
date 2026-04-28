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
 
 
int createSocket()
{
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock == -1)return -1;
    return sock;
}
int main(int argc,const char *argv[])
{
    if(argc != 3) 
    {
        std::cout << "用法: " << argv[0] << " <IP地址> <端口号>" << std::endl;
        return -1;
    }

    int send_sock = createSocket();
    if(send_sock < 0)
    {
        perror("Error");
        return -1;
    } 
 
     
    while(1)
    {
        // 目标地址
        struct sockaddr_in dest_addr;
        dest_addr.sin_family     = AF_INET;
        dest_addr.sin_port       = htons(atoi(argv[2]));
        dest_addr.sin_addr.s_addr= inet_addr(argv[1]);
        
        std::cout<<"请输入要向:"<<argv[1]<<" 发送的消息(输入exit退出)：";

        struct Package data;
        std::cin >> data.buffer;
        data.type = Package::MSG;
        data.size = strlen(data.buffer);
 
        sendto(send_sock,&data,sizeof(data),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
         
        if(std::string(data.buffer) == "exit")break;
    }
    close(send_sock);
    return 0;
}