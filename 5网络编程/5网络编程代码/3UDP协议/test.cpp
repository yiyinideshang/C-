#include <iostream>
#include <sys/socket.h> //套接字编程核心头文件
#include <sys/types.h> //定义各种系统数据类型
#include <unistd.h> //Unix 标准函数定义
#include <arpa/inet.h> //Internet 地址操作函数
#include <cstring>

int CreateSocket()
{
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock == -1)
    {
        perror("socket失败");
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
    int send_sock = CreateSocket();
    if(send_sock < 0)
    {
        perror("socket失败");
        return -1;
    }
    while(1)
    {
        std::string data;
        std::cout<<"请输入要向："<<argv[1]<<" 发送的数据,输入exit退出";
        std::cin>>data;

        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(atoi(argv[2]));
        dest_addr.sin_addr.s_addr = inet_addr(argv[1]);

        int flag;
        flag = sendto(send_sock,data.c_str(),data.size(),0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
        if(flag == -1)
        {
            perror("发送数据失败");
        }
        if(data == "exit")
            break;   
    }
    close(send_sock);
    return 0;
}