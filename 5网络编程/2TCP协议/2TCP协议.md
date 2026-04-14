# `assert()`函数

`assert()` 是一个**调试辅助工具**，用于在程序运行时检查假设条件是否成立。如果条件成立，程序继续，如果条件为假，程序会立即终止并输出错误信息。

**函数原型：**

```c++
#include <assert.h>
void assert(int expression);
```

**assert() 的核心价值：**

1. **调试工具**：帮助发现和诊断程序中的逻辑错误
2. **文档作用**：明确代码的假设和前提条件
3. **防御性编程**：及早发现违反约束的情况
4. **开发效率**：减少调试时间，提高代码质量

**黄金法则：**

- 使用 `assert` 来捕获**程序员的错误**
- 使用 `if` 来处理**运行时可能出现的错误**
- 记住：**发布版本中 `assert` 会被移除**

# **将 `sockaddr_in` 结构体清零**

- `bzero()` 

- `memset()`

- 初始化时清零

### 1. **初始化所有字段为确定值**

```c++
struct sockaddr_in {
    sa_family_t    sin_family;   // 地址族 (2字节)
    in_port_t      sin_port;     // 端口号 (2字节)
    struct in_addr sin_addr;     // IP地址 (4字节)
    char           sin_zero[8];  // 填充字节 (8字节)
};  // 总共16字节
```

清零确保所有字段从0开始，避免使用未初始化的随机值。

### 2. **为套接字绑定做准备**

```c++
// 典型用法
serveraddr.sin_family = AF_INET;          // IPv4
serveraddr.sin_port = htons(8080);        // 设置端口
serveraddr.sin_addr.s_addr = INADDR_ANY;  // 监听所有接口

// 如果不先清零，可能包含垃圾数据
bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
```

绑定到套接字时，结构体必须完全初始化。

### 3. **填充字段清零**

`sockaddr_in` 的 `sin_zero[8]` 是填充字段，用于使结构体大小与 `sockaddr` 相同。这个填充必须为0。

### 4. **三种方案**

#### 函数原型

- **bzero()函数原型：** 

```c++
// bzero - 起源于BSD，不是ANSI C标准函数
void bzero(void *s, int n);// POSIX标准（已标记为废弃）
```

- #### **为什么`POSIX`废弃bzero**

  - `memset` 已经提供了所有功能

  - 减少冗余`API`，简化标准库

  - `memset` 的优化已经足够好

- **memset()函数原型**

```c++
// memset - 标准C库函数，包含在<string.h>
void *memset(void *s, int c, size_t n);
```

| 特性         | ==`memset`==                 | `bzero`                       |
| :----------- | :--------------------------- | :---------------------------- |
| **标准性**   | ANSI C标准，可移植性高       | BSD起源，`OSIX`有但标记为废弃 |
| **参数**     | `(void *s, int c, size_t n)` | `(void *s, size_t n)`         |
| **功能**     | 可以设置任意值（不限于0）    | 只能设置为0                   |
| **返回值**   | 返回指向s的指针              | 无返回值(void)                |
| **编译环境** | 所有C编译器都支持            | 需要特定头文件或库支持        |

- **初始化时清零**

```c++
struct sockaddr_in serveraddr = {0};
```

````c++
struct sockaddr_in {
    sa_family_t    sin_family;   // 地址族 (2字节)
    in_port_t      sin_port;     // 端口号 (2字节)
    struct in_addr sin_addr;     // IP地址 (4字节)
    char           sin_zero[8];  // 填充字节 (8字节)
};  // 总共16字节

// 使用 memset（标准C函数）
memset(&serveraddr, 0, sizeof(serveraddr));
//使用 bzero
bzero(&serveraddr,sizeof(serveraddr));
// 或者初始化时清零
struct sockaddr_in serveraddr = {0};

// 典型用法
serveraddr.sin_family = AF_INET;          // IPv4
serveraddr.sin_port = htons(8080);        // 设置端口
serveraddr.sin_addr.s_addr = INADDR_ANY;  // 监听所有接口
//serveraddr.sin_addr.s_addr = inet_addr("192.168.31.1"); //监听192.168.31.1，法1
// inet_aton("192.168.31.1",&sock_info.sin_addr);//监听192.168.31.1，法2

// 如果不先清零，可能包含垃圾数据
bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
````

### 为什么必须这样做？

1. **安全性**：避免潜在的信息泄漏
2. **兼容性**：确保与 `sockaddr` 结构体大小兼容（16字节）
3. **可靠性**：避免未定义行为

在实际网络编程中，这是个必须遵循的良好习惯。

### 总结

- **新项目**：使用 `memset`，更标准、更灵活
- **维护旧代码**：可以继续使用 `bzero` 保持一致性
- **关键区别**：`memset`可以设置任意值，`bzero`只能清零
- **可移植性**：如果需要跨平台，绝对使用 `memset`。
- 现代最佳实践是使用 `memset`。

# 基于TCP套接字编程流程

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
-  发送/接收数据
  -  发送数据： `write/send/sendto`
  -  接收数据： `read/recv/recvfrom` 
- 关闭连接 
  - 四次挥手

## TCP网络应用的编程流程

![7c7447be58ab98047d2f6cd83416bd69](D:\Typora\typora_work\5网络编程\2TCP协议\流程.png)

### TCP-Server服务端

#### 建立一个套件字：（ `socket` ）	

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
        SOCK_STREAM 流式套接字 --TCP
        SOCK_DGRAM 数据报套接字
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

#### 绑定一个网络地址：( `bind` )

- 并不是任意的地址都可以（需要合法且能够正常访问）
- 把一个套接字和一个网络地址进行绑定。--》如果想让其他人来主动联系/连接，就需要绑定一个地 址，并且需要把这个地址告诉其他人。不进行绑定，不代表套接字没有地址，不进行绑定套接字在 进行通信时候，内核会动态为套接字指定一个地址。

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
            sa_family_t sin_family; // 指定地址族
            char sa_data[14];
        };
        
    	专用协议地址结构：
        struct sockaddr_in
        {
            sa_family_t sin_family; // 指定地址族 2字节
            u_int16_t sin_port; // 端口号 2字节
            struct in_addr sin_addr;// IP地址 4字节
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
    	成功返回0，失败返回-1并设置errno
*/
```

![v2-48f56688737085cf83d132b6f0462ad3_r](D:\Typora\typora_work\5网络编程\2TCP协议\v2-48f56688737085cf83d132b6f0462ad3_r.jpg)

#### 等待监听：( `listen` )

==76-78页==

让一个套接字进入一个 `监听状态`

- `listen` 只是一个配置函数，类似 `bind`，都不会阻塞。**它仅仅是将套接字设置为监听状态**，并指定**已完成连接队列**的长度（`backlog`），然后立即返回。这个过程不涉及等待任何外部事件，因此不会阻塞程序的执行。
- 它维护两个队列：
  *未完成连接队列*（`SYN_RCVD` 状态）和*已完成连接队列*（`ESTABLISHED` 状态）。
- 当**已完成连接队列不为空**时(即客户端也完成了`connect`，经过三次握手，该连接进入已完成队列)，表示**该套接字是可读的**了，

```c++
#include <sys/socket.h>

int listen(int sockfd, int backlog);
/*
    @描述：
        将 sockfd 标记为被动监听套接字，开始接受连接请求。
    @sockfd：
        需要进入监听模式的套接字（已绑定地址）。
    @backlog：
        已完成连接队列的最大长度（等待 accept 的连接的数）。
        实际值可能受系统参数 net.core.somaxconn 限制。
    @return：
        成功返回 0，失败返回 -1 并设置 errno。
*/
```

#### 等待客户端的连接：( `accept` )

 `accept` 函数默认是会**阻塞等待**，是因为它需要从已完成连接队列中取出一个已经建立好的连接`ESTABLISHED`。当队列为空（即还没有客户端完成三次握手）时，`accept` 没有连接可以返回，于是它会让进程进入睡眠状态，直到有新连接到来时被内核唤醒。

- 函数成功返回 表示和一个客户端完成连接 
- 多次调用函数就可以与不同的客户端进行连接

```c++
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
/*
    @描述：
        从 sockfd 的已完成连接队列头部取出一个连接，
        为该连接创建新的已连接套接字并返回其描述符。
        该函数不参与三次握手，握手已由内核完成。
    @sockfd：
        正在监听的套接字（已调用 listen）。
    @addr：
        输出参数，用于存储客户端的地址信息（可为 NULL）。
    @addrlen：
        输入输出参数，调用时指向 addr 结构体的长度，
        返回时保存实际客户端地址结构的长度。
    @return：
        成功返回新的已连接套接字描述符（非负），
        失败返回 -1 并设置 errno。
*/
```

#### 数据的传输： 读/写

==81-87页==

发送数据： `write/send/sendto/sendmsg` 

接收数据： `read/recv/recvfrom/recvmsg`

- 注意：**“发送”和“接收”是相对于==本端应用程序==而言的**：
  - **发送缓冲区**：用于暂存本端应用程序要发往对端的数据。当你调用 `send` 时，数据从你的应用程序缓冲区（用户空间）复制到内核中该 socket 的发送缓冲区，随后协议栈（如 TCP）会从发送缓冲区取出数据，封装成报文通过网络发出去。
  - **接收缓冲区**：用于暂存从对端接收到的数据。当对端的数据到达本端网卡后，内核协议栈将数据放入接收缓冲区，等待你的应用程序调用 `recv` 来读取。

```c
应用程序（用户空间/用户缓冲区）   内核（协议栈）           网络					数据流向
    send()  →  				[发送缓冲区]  →  	TCP封装 → 网卡 → 网络	  用户空间 → 内核空间（用户区只读）
    recv()  ←   			[接收缓冲区]  ←  	TCP解析 ← 网卡 ← 网络	   内核空间 → 用户空间（用户区可写）
```

- `send` 会从指向用户空间的缓冲区`buf`**读取**数据并复制到内核的发送缓冲区。由于它只读取不修改用户空间，所以类型是 `const void *`。
  - ——发送：把自己的`buf`缓冲区上的数据准好，发给本端内核用来发送的缓冲区(即发送缓冲区)
  - （用户自己的`buf`缓冲区（不变，只是复制）->内核发送缓冲区）

- 对端可通过`recv`将内核接收缓冲区接收到对端的用户空间上。
  - （内核接受缓冲区->用户自己的`buf`缓冲区（改变，写入自己的缓冲区））


```c++
#include <sys/types.h>
#include <sys/socket.h>
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
/*
    作用：
    	往指定套接字中写入数据
    @sockfd：
    	需要写入数据的套接字描述符
    @buf：
    	指向用户空间的缓冲区;
    @len：
    	数据的长度
    @flags:
    	一般给0，” 带外数据 “
    @return：
        成功返回实际发送的字节数，失败返回-1，并设置 errno
*/
```

- `recv` 会将内核接收缓冲区中的数据**写入**这块用户空间的缓冲区`buf`，因此缓冲区内容会被修改，所以类型是 `void *`（不带 `const`）。（——读取：把内核接收到的缓冲区的数据搬到自己的`buf`缓冲区上，让用户自己看见，`buf`缓冲区发生了变化）

```c++
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
/*
    作用：
    	从指定套接字中获取数据
    @sockfd：
    	需要读取数据的套接字描述符
    @buf：
    	指向用户空间的缓冲区
    @len：
    	需要获取的数据的长度
    @flags:
    	一般给0，” 带外数据 “
    @return：
    	成功返回实际获取的字节数，失败返回-1，并设置 errno
    recv可能返回0，这意味着通信双方已经关闭连接了。
*/
```

**flags参数**的常用值：**`MSG_OOB`**：表示发送或接收紧急数据

> 代码清单5-6 发送带外数据——客户端发送数据到服务器

==参见：D:\Typora\typora_work\Linux高性能服务器编程\5Linux网络编程基础API.md==

> 代码清单 5-7 接收带外数据——服务器接收来自客户端的数据

#### 关闭套接字：( close/shutdown )

四次挥手	==80-81页==

---

关闭一个连接实际上就是关闭该连接对应的套接字socket，可以通过关闭普通文件描述符的系统调用close来完成：

```c
#include <unistd.h>
int close(int fd);
```

**参数`fd`**：这个 参数是 **待关闭的socket**。不过，close系统调用并非总是立即关闭一个连接，而是将`fd`的引用计数减1,。只有当`fd`的引用计数为0时，才真正关闭连接。

在多进程中，一次fork系统调用默认将使父进程中打开的socket的引用计数加1，因此我们必须在父进程和子进程中都对该socket执行close调用才能将连接关闭。

---

如果无论如何都要立即终止连接(而不是将socket的引用计数减1)，可以使用**shutdown系统调用**(相对close来说，它是专门为网络编程设计的)

```c++
#include <sys/socket.h>
int shutdown(int sockfd, int how);
/*
    作用：
    	关闭一个套接字
    @sockfd：
    	需要关闭操作的套接字描述符
    @how：
    	关闭方式：
            SHUT_RD 关闭读
            SHUT_WR 关闭写。这种情况下，连接处于半关闭状态
            SHUT_RDWR 关闭读写 -->close(sockfd);
   返回值：
   		shutdown成功时返回0，失败时返回-1，并设置errn。
*/
```

### TCP-Client客户端

建立一个套接字： `socket`

#### **绑定地址：可选** 

- 可以绑定也可也不绑定（不推荐绑定，让系统分配）

#### 发起连接请求： `connect`

- 当服务器完成等待监听后，客户端调用 `connect` 发送 SYN 包，经过三次握手后，连接进入已完成队列。

```c++
CONNECT(2) 	Linux Programmer's Manual CONNECT(2)
NAME
	connect - initiate a connection on a socket
SYNOPSIS
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
/*
    @描述：
    	用指定的套接字，对指定网络地址发起连接请求
    @sockfd：
        发起连接请求的套接字
        同时这个套接字是与服务端进行数据通信的套接字
    @addr:
    	需要连接到的网络地址，目标地址
    @addrlen：
    	目标地址结构体的大小
    @return：
    	成功返回0，失败返回-1
*/
```

#### 数据的传输： 读/写 

- 发送数据： `write/send/sendto` 
- 接收数据： `read/recv/recvfrom` 

#### 关闭套接字： close/shutdown

## 示例：

### TCP_Server.cpp

- `strstr`函数

```c++
char *strstr(const char *haystack, const char *needle);
strstr 函数在 字符串haystack 中查找 字符串needle。如果找到了，函数返回指向 needle 在 haystack 中第一次出现的位置的指针；否则，返回 NULL。

```



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
    if(listen(Server_sockfd, QUEUE_SIZE) == -1) {    //监听Socket
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
            Sockfd = accept(Server_sockfd,(struct sockaddr*)&client,&len);//连接Socket
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
            int bytes_received = recv(Sockfd, recvContext, 1023, 0);//接收buff为：recvContext
            if(bytes_received > 0) {
                recvContext[bytes_received] = '\0'; // 确保字符串终止
            }
            //将接收到的数据进行处理
            std::string context;//发送buff为：context
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
                {//如果客户端发送再见信息，直接将context“下次见”信息，发送给客户端然后结束循环。
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
            
            //服务器将要客户端发送的send内容进行recv接收后并处理，将处理后的信息context内容 再发送给客户端
            if(send(Sockfd,context.c_str(),context.size(),0) == -1)//第二个参数传递const，服务器将自己的发送缓冲区 context 发送给客户端
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
            std::string context;//发送buff:准备要发送给服务器的内容
            std::cout<<"请输入:";
            std::getline(std::cin, context); // 读取整行

            // 客户端先发送后接收，完成一次完整的"问答"
            if(send(Client_sockfd,context.c_str(),context.size(),0) == -1)//第二个参数传递 const,客户端将自己的发送缓冲区 context发送给服务器。
            {
                perror("向服务器发送失败");
                break;
            }

            // recv()，从服务器接受信息存储到recvContext中，会阻塞等待，直到服务器有数据返回或连接断开
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

# ==-----------------------==

# **atoi() 函数详解**

`atoi()` 是 C 标准库中的一个**字符串转换函数**，用于将**字符串转换为整数**。它的名字来源于 **"ASCII to integer"**（ASCII 转整数）。

## 基本概念

### 函数原型

```c
#include <stdlib.h>  // 需要包含头文件
int atoi(const char *str);
```

### 功能描述

`atoi()` 解析字符串 `str`，将其转换为整数并返回。

```c
// 简单示例
#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *str1 = "123";
    const char *str2 = "-456";
    const char *str3 = "   789";   // 包含空格
    const char *str4 = "123abc";   // 包含非数字字符
    
    printf("atoi(\"%s\") = %d\n", str1, atoi(str1));  // 123
    printf("atoi(\"%s\") = %d\n", str2, atoi(str2));  // -456
    printf("atoi(\"%s\") = %d\n", str3, atoi(str3));  // 789
    printf("atoi(\"%s\") = %d\n", str4, atoi(str4));  // 123（只转换到非数字字符）
    
    return 0;
}
```

## 工作原理

### 转换规则

1. **跳过前导空白字符**（空格、制表符、换行符等）
2. **识别可选的正负号**（+ 或 -）
3. **读取连续的数字字符**（0-9）
4. **遇到第一个非数字字符时停止**
5. **返回转换后的整数值**

```c
// 模拟 atoi() 的工作过程
int my_atoi(const char *str) {
    int result = 0;
    int sign = 1;  // 符号：1表示正数，-1表示负数
    int i = 0;
    
    // 1. 跳过前导空白字符
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
           str[i] == '\r' || str[i] == '\f' || str[i] == '\v') {
        i++;
    }
    
    // 2. 检查符号
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    
    // 3. 转换数字字符
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return sign * result;
}
```

## 重要特性

### 1. **错误处理能力差（最大缺点）**

```c
// atoi() 没有错误检测机制
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 问题1：空指针（会导致段错误）
    // int val1 = atoi(NULL);  // 崩溃！
    
    // 问题2：无效输入返回0（无法区分是真的0还是错误）
    printf("atoi(\"abc\") = %d\n", atoi("abc"));      // 0
    printf("atoi(\"0\") = %d\n", atoi("0"));          // 0
    // 无法区分是转换失败还是字符串本身就是"0"
    
    // 问题3：溢出行为未定义
    printf("atoi(\"99999999999999999999\") = %d\n", 
           atoi("99999999999999999999"));  // 溢出，结果未定义
    
    return 0;
}
```

### 2. **支持有限的非数字内容**

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    // 会自动跳过前导空白
    printf("atoi(\"   123\") = %d\n", atoi("   123"));    // 123
    
    // 遇到非数字字符停止
    printf("atoi(\"123abc456\") = %d\n", atoi("123abc456"));  // 123
    printf("atoi(\"12.34\") = %d\n", atoi("12.34"));          // 12（遇到小数点停止）
    printf("atoi(\"123 456\") = %d\n", atoi("123 456"));      // 123（遇到空格停止）
    
    // 空字符串或没有数字的字符串返回0
    printf("atoi(\"\") = %d\n", atoi(""));                // 0
    printf("atoi(\"  \") = %d\n", atoi("  "));            // 0（只有空白）
    
    // 支持十六进制？不支持！
    printf("atoi(\"0xFF\") = %d\n", atoi("0xFF"));        // 0（遇到'0'停止）
    printf("atoi(\"FF\") = %d\n", atoi("FF"));            // 0
    
    return 0;
}
```

## 更安全的替代函数

### 1. **strtol() - 最推荐的替代品**

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

void test_strtol(const char *str) {
    char *endptr;
    long value;
    
    // 重置错误标志
    errno = 0;
    
    // 转换字符串
    value = strtol(str, &endptr, 10);  // 十进制转换
    
    printf("\n转换字符串: \"%s\"\n", str);
    
    // 检查转换是否成功
    if (str == endptr) {
        printf("错误: 没有转换任何数字\n");
    } else if (*endptr != '\0') {
        printf("警告: 字符串包含额外字符: \"%s\"\n", endptr);
        printf("成功转换部分: %ld\n", value);
    } else {
        printf("成功转换: %ld\n", value);
    }
    
    // 检查溢出
    if (errno == ERANGE) {
        if (value == LONG_MAX)
            printf("错误: 上溢 (值太大)\n");
        else if (value == LONG_MIN)
            printf("错误: 下溢 (值太小)\n");
    }
}

int main() {
    test_strtol("123");
    test_strtol("   -456");
    test_strtol("123abc");
    test_strtol("999999999999999999999999999");
    test_strtol("abc");
    test_strtol("");
    
    return 0;
}
```

### 2. **strtoul() - 无符号版本**

```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void test_strtoul(const char *str) {
    char *endptr;
    unsigned long value;
    
    errno = 0;
    value = strtoul(str, &endptr, 10);
    
    if (errno == ERANGE) {
        printf("'%s' 溢出\n", str);
    } else if (*endptr != '\0') {
        printf("'%s' 包含非数字字符\n", str);
    } else {
        printf("'%s' -> %lu\n", str, value);
    }
}
```

### 3. **atoi 家族的其他成员**

```c
// 不同类型的转换函数
#include <stdlib.h>

// 字符串转长整型
long atol(const char *str);

// 字符串转长长整型（C99）
long long atoll(const char *str);

// 字符串转双精度浮点数
double atof(const char *str);

// 示例
int main() {
    printf("atol(\"1234567890\") = %ld\n", atol("1234567890"));
    printf("atoll(\"1234567890123456789\") = %lld\n", atoll("1234567890123456789"));
    printf("atof(\"3.14159\") = %f\n", atof("3.14159"));
    
    return 0;
}
```

## 实际应用中的注意事项

### 何时使用 atoi()

```c
// 适合使用 atoi() 的场景：
// 1. 完全信任输入数据
// 2. 简单原型或测试代码
// 3. 已知输入格式完全正确

// 示例：配置文件读取已知格式
int parse_config_simple(const char *value_str) {
    // 已知配置文件格式正确，只包含数字
    return atoi(value_str);
}

// 示例：处理命令行参数（简单情况）
int main(int argc, char *argv[]) {
    if (argc > 1) {
        // 假设用户输入正确
        int repeat_count = atoi(argv[1]);
        for (int i = 0; i < repeat_count; i++) {
            printf("Hello %d\n", i);
        }
    }
    return 0;
}
```

### 何时避免使用 atoi()

```c
// 应该避免使用 atoi() 的场景：
// 1. 用户输入（不可信数据）
// 2. 需要错误处理
// 3. 需要检测溢出
// 4. 需要知道转换了多少字符

// 不安全的用户输入处理（错误示例）
int get_user_age_unsafe() {
    char input[100];
    printf("请输入年龄: ");
    fgets(input, sizeof(input), stdin);
    
    // 危险！用户可能输入非数字
    int age = atoi(input);
    return age;  // 如果用户输入"abc"，返回0，但可能不是用户的本意
}

// 安全的用户输入处理（正确示例）
int get_user_age_safe() {
    char input[100];
    char *endptr;
    long age;
    
    printf("请输入年龄: ");
    fgets(input, sizeof(input), stdin);
    
    // 使用 strtol 进行安全转换
    age = strtol(input, &endptr, 10);
    
    // 验证转换
    if (endptr == input) {
        printf("错误: 没有输入数字\n");
        return -1;
    }
    
    if (*endptr != '\n' && *endptr != '\0') {
        printf("警告: 输入包含非数字字符\n");
    }
    
    if (age < 0 || age > 150) {
        printf("错误: 年龄范围无效 (0-150)\n");
        return -1;
    }
    
    return (int)age;
}
```

## 性能比较

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 1000000

void performance_test() {
    const char *test_string = "123456789";
    clock_t start, end;
    long total;
    
    // 测试 atoi
    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        total += atoi(test_string);
    }
    end = clock();
    printf("atoi 耗时: %.4f 秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // 测试 strtol
    char *endptr;
    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        total += strtol(test_string, &endptr, 10);
    }
    end = clock();
    printf("strtol 耗时: %.4f 秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
    // 测试自定义 atoi（无错误检查）
    start = clock();
    for (int i = 0; i < ITERATIONS; i++) {
        // 简单实现，类似atoi
        int result = 0;
        const char *p = test_string;
        while (*p >= '0' && *p <= '9') {
            result = result * 10 + (*p - '0');
            p++;
        }
        total += result;
    }
    end = clock();
    printf("自定义 atoi 耗时: %.4f 秒\n", (double)(end - start) / CLOCKS_PER_SEC);
}
```

## C++ 中的替代

```cpp
#include <iostream>
#include <string>
#include <sstream>

// 方法1：使用stoi（推荐）
int string_to_int_cpp(const std::string& str) {
    try {
        size_t pos;
        int value = std::stoi(str, &pos);
        
        // 检查是否整个字符串都被转换
        if (pos != str.length()) {
            std::cout << "警告: 部分转换" << std::endl;
        }
        return value;
    } catch (const std::invalid_argument& e) {
        std::cerr << "无效参数: " << e.what() << std::endl;
        return 0;
    } catch (const std::out_of_range& e) {
        std::cerr << "超出范围: " << e.what() << std::endl;
        return 0;
    }
}

// 方法2：使用stringstream
int string_to_int_stream(const std::string& str) {
    std::stringstream ss(str);
    int value;
    ss >> value;
    
    if (ss.fail()) {
        std::cerr << "转换失败" << std::endl;
        return 0;
    }
    
    return value;
}
```

## 最佳实践总结

### ✅ 可以使用 atoi() 的情况

```c
// 1. 内部数据转换（完全可信）
const char *internal_value = "100";
int timeout = atoi(internal_value);  // 配置文件中的已知值

// 2. 简单的测试和原型代码
// 3. 性能关键且输入格式已知正确的场景
```

### ⚠️ 应该使用更安全函数的情况

```c
// 永远使用这些替代方案处理不可信输入：

// 1. 用户输入
int get_integer_input(const char *prompt) {
    char buffer[100];
    char *endptr;
    long value;
    
    printf("%s", prompt);
    fgets(buffer, sizeof(buffer), stdin);
    
    // 移除换行符
    buffer[strcspn(buffer, "\n")] = '\0';
    
    value = strtol(buffer, &endptr, 10);
    
    // 详细错误检查
    if (endptr == buffer) {
        fprintf(stderr, "错误: 没有数字\n");
        return -1;
    }
    
    if (*endptr != '\0') {
        fprintf(stderr, "警告: 额外字符: %s\n", endptr);
    }
    
    if (value < INT_MIN || value > INT_MAX) {
        fprintf(stderr, "错误: 值超出int范围\n");
        return -1;
    }
    
    return (int)value;
}

// 2. 网络数据
int parse_network_packet(const char *packet_data) {
    return (int)strtol(packet_data, NULL, 10);
}

// 3. 文件输入
int read_config_value(FILE *config_file) {
    char line[256];
    if (fgets(line, sizeof(line), config_file)) {
        return (int)strtol(line, NULL, 10);
    }
    return 0;
}
```

## 常见陷阱

### 陷阱1：无法区分 "0" 和转换失败

```c
int value1 = atoi("0");      // 返回 0
int value2 = atoi("abc");    // 也返回 0
// 无法区分这两种情况！
```

### 陷阱2：溢出行为未定义

```c
// 超过 int 范围的字符串
int overflow = atoi("99999999999999999999");
// 结果取决于编译器和平台，可能是未定义行为
```

### 陷阱3：不支持十六进制或八进制

```c
int hex = atoi("0xFF");      // 返回 0（遇到 '0' 停止）
int oct = atoi("0777");      // 返回 777（但作为十进制）
// 这些都不是预期的十六进制/八进制转换
```

## 总结

**`atoi()` 的特点：**

- **简单易用**：单行代码即可完成转换
- **性能较高**：比 strtol 等函数更快
- **缺乏安全性**：没有错误检测机制
- **过时**：现代C编程中不推荐用于生产代码

**推荐做法：**

1. **学习和理解** `atoi()` 的工作原理
2. **原型开发**可以使用 `atoi()` 快速验证想法
3. **生产代码**总是使用 `strtol()` 系列函数
4. **C++代码**使用 `std::stoi()` 或 `std::strtol()`
5. **输入验证**是必不可少的，永远不要信任外部输入

记住这个黄金法则：**"如果你不100%确定输入格式完全正确，就不要使用 atoi()"**。
