#include <iostream>
#include <sys/socket.h> //套接字编程核心头文件
#include <sys/types.h> //定义各种系统数据类型
#include <unistd.h> //Unix 标准函数定义
#include <arpa/inet.h> //Internet 地址操作函数
#include <cstring>

int CreateSocket(std::string ipv4,int port)
{
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock == -1)
    {
        perror("socket失败");
        return -1;
    }

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ipv4.c_str());
    int res = bind(sock,(struct sockaddr *)&local,sizeof(local));
    if(res == -1)
    {
        perror("绑定失败");
        return -1;
    }
    return sock;
}

int main(int argc,char* argv[2])
{
    if(argc != 3)
    {
        std::cout<<"用法："<<argv[1]<<"+ IP地址 + 端口号"<<std::endl;
        return -1;
    }

    int recv_sock = CreateSocket(argv[1],atoi(argv[2]));
    if(recv_sock < 0)
    {
        perror("socket失败");
        return -1;
    }
    std::cout<<"正在接受数据<"<<std::endl;
    while(1)
    {
        char buff[104] = {0};
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
        int flag = recvfrom(recv_sock,buff,1024,0,(struct sockaddr*)&sender,&size);
        if(flag == -1)
        {
            perror("接受数据失败");
            return -1;
        }
        if(std::string(buff) == "exit")
            break;
        std::cout<<"来自["<<inet_ntoa(sender.sin_addr)<<":"<<
        ntohs(sender.sin_port)<<"]"<<"的消息"<<std::endl;
    }
    close(recv_sock);
    return 0;
}