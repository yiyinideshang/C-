#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#define QUEUE_SIZE 10

int CreateTCPClient(std::string addr, int port)
{
    // 申请一个套接字
    int Client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(Client_sockfd == -1) {
        perror("socket失败");
        return -1;
    }

    //连接服务器地址,【main 函数的两个参数-》表示服务器IP地址和端口号】
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(addr.c_str());
    server.sin_port = htons(port);

    if(connect(Client_sockfd,(struct sockaddr*)&server,sizeof(server)) == -1)
    {
        perror("连接失败");
        close(Client_sockfd);
        return -2;
    }

    return Client_sockfd;
}

int main(int argc, const char* argv[])
{
    if(argc != 3) {
        std::cout << "用法: " << argv[0] << " <IP地址> <端口号>" << std::endl;
        return -1;
    }
    
    int Client_sockfd = CreateTCPClient(argv[1], atoi(argv[2]));
    if(Client_sockfd < 0) {
        std::cout << "客户端启动失败" << std::endl;
        perror("套接字创建失败");
    } else {
        std::cout << "套接字连接成功，可以通信" << std::endl;
    
        while(1)
        {
            std::string context;
            std::cout<<"请输入:";
            std::getline(std::cin, context); // 读取整行

            // 客户端先发送后接收，完成一次完整的"问答"
            if(send(Client_sockfd,context.c_str(),context.size(),0) == -1)
            {
                perror("向服务器发送失败");
                break;
            }

            // recv()，从服务器接受信息，会阻塞等待，直到服务器有数据返回或连接断开
            char recvContext[1024] = {0};//接收缓冲区在栈上分配，每次循环重新初始化
            if(recv(Client_sockfd,recvContext,1023,0) == -1)
            {
                perror("从服务器接受信息失败");
                break;
            }
            std::cout<<"来自服务器的消息："<<recvContext<<std::endl;
            if(strstr(recvContext,"下次见"))
            {
                std::cout<<"服务器确认断开连接"<<std::endl;
                usleep(1000000); // 等待1000ms表示加载动画
                std::cout<<"会话结束..."<<std::endl;
                break;
            }
// 用户输入 → 客户端send() → 网络 → 服务器recv()
// 服务器处理 → 服务器send() → 网络 → 客户端recv() → 显示给用户
        }
    }

    close(Client_sockfd);
    return 0;
}