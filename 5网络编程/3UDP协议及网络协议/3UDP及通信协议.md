# ==基于`TCP`套接字编程流程==

任何网络应用都会有通信双方：

- `Send` 发送端 
- `recv` 接收端

`TCP` **网络应用**（**C/S模型**）（长连接）

- `Client` 客户端（ TCP ） 
- `Server` 服务端（ TCP ）

**任何的网络应用： **

- 传输层的协议（ `TCP/UDP` ）+ 端口 + `IP` 地址

**网络地址**： 

- 任何网络应用任意一方都需要有一个 **网络地址** （ `IP`+端口 ）



## TCP网络应用执行的过程

- 建立连接 
  - 三次握手
- 发送/接收数据
  -  发送数据： `write/send/sendto`
  -  接收数据： `read/recv/recvfrom` 
- 关闭连接 
  - 四次挥手

## TCP网络应用的编程流程

![流程](D:\Typora\typora_work\5网络编程\2TCP协议\流程.png)

### TCP-Server服务端

#### 建立一个套件字：（ `socket` ）

```c++

```

#### 绑定一个网络地址：( `bind` )

- 并不是任意的地址都可以（需要合法且能够正常访问）
- 把一个套接字和一个网络地址进行绑定。--》如果想让其他人来主动联系/连接，就需要绑定一个地 址，并且需要把这个地址告诉其他人。不进行绑定，不代表套接字没有地址，不进行绑定套接字在 进行通信时候，内核会动态为套接字指定一个地址。

```c++

```

![v2-48f56688737085cf83d132b6f0462ad3_r](D:\Typora\typora_work\5网络编程\3UDP协议及网络协议\v2-48f56688737085cf83d132b6f0462ad3_r.jpg)

#### 等待监听：( `listen` )

让一个套接字进入一个 `监听状态`

```c++

```

#### 等待客户端的连接：( `accept` )

- 等待客户端来发起连接和客户端建立 `TCP` 连接 
  - 三次握手 
- 函数成功返回 表示和一个客户端完成连接 
- 多次调用函数就可以与不同的客户端进行连接

```c++

```

#### 数据的传输： 读/写

发送数据： `write/send/sendto` 

接收数据： `read/recv/recvfrom`

```c++

```

```c++

```

#### 关闭套接字：( close/shutdown )

四次挥手	

```c++

```

### TCP-Client客户端

建立一个套接字： `socket`

#### **绑定地址：可选** 

- 可以绑定也可也不绑定（不推荐绑定，让系统分配）

#### 发起连接请求： `connect`

```c++

```

#### 数据的传输： 读/写 

- 发送数据： `write/send/sendto` 
- 接收数据： `read/recv/recvfrom` 

#### 关闭套接字： close

## 示例：

### TCP_Server.cpp

```c++
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
```

### TCP_Client.cpp

```c++
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
```

```c++
g++ TCP_Server.cpp -o TCP_Server

g++ TCP_Client.cpp -o TCP_Client

./TCP_Server 192.168.201.128 8899

./TCP_Client 192.168.201.128 8899
```

![屏幕截图 2025-11-05 202133](D:\Typora\typora_work\5网络编程\2TCP协议\屏幕截图 2025-11-05 202133.png)

# 作业：

尝试将`TCP`的操作封装成一个类

# ---------------------------

# ==基于`UDP`套件字编程流程==

`UDP` 传输层的协议，面向无连接，数据报的传输层协议。

“ **无连接** ”：不可靠

- 在网络环境较好的情况下， `UDP` 效率较高 
- 在网络环境较差的情况下， `UDP` 可能存在丢包的情况 
- 同时一些“ 实时应用 ” 采用 `UDP` 
- 在应用层加一些保证传输可靠的“ **控制协议** ”

`UCP` **网络应用**

- `Recver` 发送端（ `TCP` ） 
- `Sender` 接收端（ `TCP` ）

## `UDP` 网络应用的编程流程

### UDP Recver(接收端):

#### **创建一个套接字** (socket)

同样使用 `socket` 接口申请一个套接字 

- `SOCK_DGRAM` 类型的套接字

  ```c++
  SOCKET(2) 	Linux Programmer's Manual
  SOCKET(2)
      
  NAME
  	socket - create an endpoint for communication
  SYNOPSIS
          
  #include <sys/types.h> /* See NOTES */
  #include <sys/socket.h>
  #include <unistd.h>
  // windows的socket在 winsock2.h
  int socket(int domain, int type, int protocol);
  /*
      @描述：
      	申请一个指定类型和指定协议的套接字
      @domain：
          指定域/协议簇。socket接口不仅不局限于TCP/IP，它可以用于Bluetooth(蓝牙)、本
          地通信...
     	 	每一种下面都有自己的许多协议，我们把IPV4下面的所有协议都归纳到了一个域：
              AF_INET IPV4
              AF_INET6 IPV6
              AF_UNIX AF_LOCAL 本地通信
              AF_BULETOOTH 蓝牙
              ...
      @type：
          指定要创建的套件字的类型：
          SOCK_STREAM 流式套接字 --TCP（stream）
          SOCK_DGRAM 数据报套接字 --UDP (dgram)
          SOCK_RAW 原始套接字
          ...
          TCP采用流式套接字，UDP采用数据报套接字
      @protocol
      	协议，指定具体的应用层协议，可以指定为0:表示采用不知名的私有的应用层
      @return：
          成功返回一个套接字描述符sockfd	
          失败返回-1，同时errno被设置	
  */
  ```

#### **绑定网络地址** (bind)

同样使用 `bind` 接口将一个套接字和一个网络地址进行绑定

```c++
BIND(2) 	Linux Programmer's Manual
BIND(2)
NAME
	bind - bind a name to a socket
SYNOPSIS
    
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
/*
    @描述：
    	用于给一个指定的套接字绑定网络地址
    @sockfd：
    	需要绑定地址的套接字
    @addr：
        一个结构体类型，表示网络地址
        socket接口不仅可以用于以太网（IPV4），也可以用于IPV6，同时也可以用于Bluetooth，....
        不同的协议簇，它的地址是不一样的。
    	socket编程接口，用一个通用的 “ 网络地址接口 ”
        struct sockaddr
        {
            sa_family_t sin_family; // 指定协议簇
            char sa_data[14];
        };
        
    	协议地址结构：
        struct sockaddr_in
        {
            sa_family_t sin_family; // 指定协议簇 2字节
            u_int16_t sin_port; // 端口号 2字节
            struct in_addr sin_addr;// IP地址 2字节
            char sin_zero[8]; // 填充8字节，为了和其他协议簇地址结构体大小一样
        };
            如：
                struct sockaddr_in sock_info;
            	sock_info.sin_family = AF_INET; // 指定为IPV4
            	sock_info.sin_port = htons(6666); //指定为6666端口
            	sock_info.sin_addr.s_addr = inet_addr("192.168.31.1"); // 绑定ip地址
            	// inet_aton("192.168.31.1",&sock_info.sin_addr);
            	
            	//将sock_info(sockaddr_in类型的)强制类型转换为 通用的socket(struct sockaddr)编程接口
            	bind(sock,(struct sockaddr *)&sock_info,sizeof(sock_info));
    @addrlen
    	表示网络地址结构体的大小
    @return：
    	成功返回0，失败返回-1
*/
```

#### 数据通信 ——接收	`recvfrom` 

只是比`recv`和`send`多了两个参数，`UDP`的这两个系统调用也可以用于面向连接（`STREAM`）的socket数据读写，只需要把最后两个都设置为NULL 以忽略 发送端/接收端的socket地址（因为我们已经建立了和对方的连接，所以已经知道其socket地址了）

- ```c++
  RECV(2) 		Linux Programmer's Manual
  RECV(2)
  NAME
  	recv, recvfrom, recvmsg - receive a message from a socket
  SYNOPSIS
  #include <sys/types.h>
  #include <sys/socket.h>
  ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
  /*
      @描述：
      	从一个套接字中接收数据
      @sockfd：
      	接收数据的套接字
      @buf：
      	接收到的数据所存放的位置/空间
      @len：
      	接收数据的空间有多大
      @flags：
          标志位：
          一般为0
      @src_addr:
          指向网络地址信息的结构体空间，用来存储发送者信息的结构体，可以为
          NULL，表示不关系发送者信息
      @addrlen：
      	指向网络地址信息结构体的空间大小的指针
      @return：
          成功返回实际读取字节数
          失败返回-1
  */
  
  ```

`struct sockaddr *src_addr`： **这是关键参数！** 它是一个指向`sockaddr`结构体的指针。当函数返回时，系统会**自动**将**发送方的地址信息**（IP和端口）填充到这个结构体中。如果不需要知道发送者，可以设为NULL。

`socklen_t *addrlen`设计成指针是因为：

1. **接收时不确定地址类型**：
   - 不知道发送方的地址是IPv4、IPv6还是其他类型
   - 需要根据返回的实际地址调整处理方式
2. **可变长度地址结构**：
   - IPv4的 `struct sockaddr_in` 大小是16字节
   - IPv6的 `struct sockaddr_in6` 大小是28字节
   - 其他协议可能有不同大小
3. **历史兼容性**：
   - `recvfrom()`需要处理各种地址族（AF_UNIX、AF_INET、AF_INET6等）
   - 通过指针返回值，可以正确处理任何大小的地址结构

它是一个**输入输出参数**(**IN/OUT parameter**)

1. **输入时**：你告诉内核`src_addr`缓冲区的最大容量
2. **输出时**：内核告诉你实际填充的地址结构大小

#### **关闭套接字**

`close` 关闭使用完的套接字

### UDP Sender(发送端):

#### **创建一个套接字**

同样使用 `socket` 接口申请一个套接字

-  `SOCK_DGRAM` 类型的套接字

#### **数据通信**——发送 `sendto`

```c++
SEND(2)	Linux Programmer's Manual
SEND(2)
NAME
	send, sendto, sendmsg - send a message on a socket
SYNOPSIS
#include <sys/types.h>
#include <sys/socket.h>
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
/*
    @描述：
    	通过一个合法套接字往一个指定的地址发送一条消息
    @sockfd:
    	网络套接字
    @buf：
    	需要发送的内容指针
    @len：
    	需要发送的内容长度
    @flags：
        标志位：
        一般采用0
    @dest_addr：
    	目标地址，接收方的网络地址
    @addrlen：
    	目标网络地址结构体的大小
    @return：
        成功返回实际发送的字节数
        失败返回-1
*/
```

`const struct sockaddr *dest_addr`： **这是一个至关重要的参数**。它是一个指向`sockaddr`结构体的指针，里面包含了**目标主机的IP地址和端口号**。

 `socklen_t addrlen`: 	`addrelen`不是 指针是因为：	**`addrlen` 只是输入参数（IN parameter）：**

- 你只需要告诉内核目标地址结构的大小
- 内核不需要修改这个值

#### 关闭套接字 

`close` 关闭使用完的套接字

## 示例1：

### UDP_Server.cpp

```c++
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
        std::string data;
        std::cout<<"请输入要向:"<<argv[1]<<" 发送的消息(输入exit退出)：";
        std::cin >> data;
 
        // 目标地址
        struct sockaddr_in dest_addr;
        dest_addr.sin_family     = AF_INET;
        dest_addr.sin_port       = htons(atoi(argv[2]));
        dest_addr.sin_addr.s_addr= inet_addr(argv[1]);
        //发送消息
        sendto(send_sock,data.c_str(),data.size(),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
        if(data == "exit")break;
    }
    close(send_sock);
    return 0;
}
```

### UDP_Recver.cpp

```c++
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
        char buff[1024]={0};
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
        // 接收消息
        recvfrom(recv_sock,buff,1024,0,(struct sockaddr*)&sender,&size);
        if(std::string(buff) == "exit")break;
        std::cout << "来自["<< inet_ntoa(sender.sin_addr) <<
        ":"<<ntohs(sender.sin_port)<<
        "]的消息：" << buff<< std::endl;
    }
    close(recv_sock);
    return 0;
}
```

## 示例2：

### UDP_Server_Agreenment.cpp

```c++
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
```

### UDP_Recver_Agreenment.cpp

```c++
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
```

## 示例3：

### UDP_Server_Packge.cpp

```c++
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
```

### UDP_Recver_Packge.cpp

```c++
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
        struct Package data;
        struct sockaddr_in sender;
        socklen_t size = sizeof(sender);
 
        recvfrom(recv_sock,&data,sizeof(data),0,(struct sockaddr*)&sender,&size);
 
        if(std::string(data.buffer) == "exit")break;
        std::cout << "来自["<< inet_ntoa(sender.sin_addr) <<"]的消息：" << data.buffer<< std::endl;
    }
    close(recv_sock);
    return 0;
}
```

## 示例4：

### UDP_Server_ReSend.cpp

```c++
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
```

### UDP_Recver_ReSend.cpp

```c++
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
```

# 网络通信协议

协议：就是通信双方约定好的通信规则。

将数据拆分成一个一个的字节，每次发一个字节。

 开始 和 结束一组特殊字符。 

中间的数据就是内容。

![屏幕截图 2025-11-05 221226](D:\Typora\typora_work\5网络编程\3UDP协议及网络协议\屏幕截图 2025-11-05 221226.png)

```c++
struct package
{
    // 数据包序号
    int number;
    // 数据的类型
    enum
    {
        CMD,
        MSG,
        IMG
    }type;
    
    // 数据的实际大小
    int size;
    // 数据内容
    char buffer[1024];
};
```

# 作业： 

写一个发送端和接收端 

- 功能：可以发送文件

# ---------------------------
