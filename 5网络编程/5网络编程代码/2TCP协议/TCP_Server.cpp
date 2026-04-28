#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#define QUEUE_SIZE 10

int CreateTCPServer(std::string addr, int port)
{
    // 申请一个套接字
    int Server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(Server_sockfd == -1) {
        perror("socket失败");
        return -1;
    }

    // 绑定网络地址 和 端口号
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(addr.c_str());//将第一个参数作为IP地址传入
    local.sin_port = htons(port);//将第二个参数作为端口号传入

    int res = bind(Server_sockfd, (struct sockaddr*)&local, sizeof(local));
    if(res == -1) {
        perror("bind失败");
        close(Server_sockfd);
        return -2;//表示绑定失败
    }
    
    // 进入监听状态
    if(listen(Server_sockfd, QUEUE_SIZE) == -1) {    
        perror("listen失败");
        close(Server_sockfd);
        return -3;//表示监听失败
    }
    
    return Server_sockfd;//返回该套接字
}

int main(int argc, const char* argv[])
{
    if(argc != 3) {
        std::cout << "用法: " << argv[0] << " <IP地址> <端口号>" << std::endl;
        return -1;
    }

    // 将第一个参数作为IP地址传入，将第二个参数作为端口号传入
    int Server_sockfd = CreateTCPServer(argv[1], atoi(argv[2]));//接受该套接字
    if(Server_sockfd < 0) {
        std::cout << "服务器启动失败" << std::endl;
        perror("套接字创建失败");
    } else {
        std::cout << "套接字创建成功，服务器已启动" << std::endl;
    
        //接受客户端连接
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int Sockfd;   
        while(1)
        {
            // 返回与该客户端的连接套接字的描述符（客户端的Client_sockfd = Sockfd）
            Sockfd = accept(Server_sockfd,(struct sockaddr*)&client,&len);
            if(Sockfd != -1)
            {
                break;
            }
        }

        //与客户端进行通信
        while(1)
        {
            //接收(recv)服务器send发送的context消息到recvContext中
            char recvContext[1024] = {0};
            int bytes_received = recv(Sockfd, recvContext, 1023, 0);
            if(bytes_received > 0) {
                recvContext[bytes_received] = '\0'; // 确保字符串终止
            }
            //将接收到的数据进行处理
            std::string context;
            if(strstr(recvContext,"你好")||strstr(recvContext,"hello"))
            {
                context = "你好！";
            }
            else if(strstr(recvContext,"你叫什么")||strstr(recvContext,"你的名字")||strstr(recvContext,"你是谁"))
            {
                context = "我是服务器端Deep";
            }
            else if(strstr(recvContext,"再见")||strstr(recvContext,"拜拜"))
            {
                std::cout << "客户端说再见，正在发送确认消息并结束会话..." << std::endl;
                context = "下次见";
                if(send(Sockfd,context.c_str(),context.size(),0) == -1)
                {
                    perror("发送告别消息失败");
                    break;
                }
                else
                {
                    // 只有发送成功才等待
                    usleep(5000000); // 等待5000ms让服务器端消息发送完成
                    std::cout << "已发送确认，结束会话..." << std::endl;
                }
                break;
            }
            else
                context = std::string("不知道你说的:[")+ recvContext + std::string("]这是什么意思");
            
            //服务器将要客户端发送的send内容进行recv接收后并处理，将context内容再send返回给客户端
            if(send(Sockfd,context.c_str(),context.size(),0) == -1)
            {
                perror("向客户端返回信息失败");
                break;
            }
        }

        close(Sockfd);
        close(Server_sockfd);
    }

    return 0;
}