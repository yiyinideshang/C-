#pragma once //是一个 C/C++ 预处理指令，用于防止同一个头文件被多次包含（重复包含）。
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#define PORT 8899
// INADDR_LOOPBACK (127.0.0.1) - 仅本地回环
// INADDR_BROADCAST (255.255.255.255) - 广播地址
// INADDR_NONE (255.255.255.255) - 错误指示
// INADDR_ANY (0.0.0.0) -服务器绑定所有网络接口的连接请求

class TcpSocket
{
private:
    int _m_sockID;
    bool _m_state;
    std::string _m_ipv4;
    int _m_port;
public:
    TcpSocket()//默认构造函数
        :_m_sockID(-1),_m_ipv4(INADDR_ANY),_m_port(PORT),_m_state(false){}
    TcpSocket(std::string ipv4,int port)
        :_m_sockID(-1),_m_ipv4(ipv4),_m_port(port),_m_state(false){}
    TcpSocket(const TcpSocket& object)
        :_m_sockID(object._m_sockID),_m_ipv4(object._m_ipv4),
        _m_port(object._m_port),_m_state(false){}
    ~TcpSocket()
    {
        close(_m_sockID);
    }
    bool Connect(std::string ipv4 =INADDR_ANY,int port = PORT)
    {
        _m_sockID = socket(AF_INET,SOCK_STREAM,0);
        if(_m_sockID == -1) return bool;

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);


    }
};

