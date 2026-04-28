#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
  
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
        char data[5] = {">se<"};
        char buff[1024] = {0};
        std::cin>>buff;

        //开始数据(报头)
        sendto(send_sock,&data[0],1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        sendto(send_sock,&data[1],1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        
        //通过循环发送
        for(int i = 0;buff[i] != '\0';i++)
        {
            sendto(send_sock,&buff[i],1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        }

        //结束数据(报尾)
        sendto(send_sock,&data[2],1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        sendto(send_sock,&data[3],1,0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));

        if(std::string(buff) == "exit")break;
    }
    close(send_sock);
    return 0;
}