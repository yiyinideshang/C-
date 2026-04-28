#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
  
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
        char data[2]={0};
        char buff[1024]={0};
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
 
        // 接收开始数据
        recvfrom(recv_sock,&data,1,0,(struct sockaddr*)&sender,&size);
        if(data[0] != '>')continue;
        std::cout << "<";
        recvfrom(recv_sock,&data[1],1,0,(struct sockaddr*)&sender,&size);
        if(data[1] != 's')continue;
        std::cout << "s:";
 
        for(int i = 0;i < 1024;i++)
        {
            // 接收消息
            recvfrom(recv_sock,&data[0],1,0,(struct sockaddr*)&sender,&size);
 
            // 判断是不是结束数据(报尾)
            if(data[0] == 'e')
            {
                recvfrom(recv_sock,&data[1],1,0,(struct sockaddr*)&sender,&size);
                if(data[1] == '<')
                {
                    std::cout << ":e>" << std::endl;
                    break;
                }
                buff[i] = data[0];
                buff[i+1] = data[1];
                i += 1;
            }
            else{
                 buff[i] = data[0];
            }
 
        if(std::string(buff) == "exit")break;
        std::cout << "来自["<< inet_ntoa(sender.sin_addr) <<"]的消息：" << buff<< std::endl;
        }
    }
    close(recv_sock);
    return 0;
}