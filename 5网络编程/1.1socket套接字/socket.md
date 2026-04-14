# open函数

## 函数原型

```c
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
```

## 参数及返回值

**参数**

- `pathname`：文件路径
- `flags`：打开方式（必选一个基本模式，可与其他标志按位或组合） 基本模式：O_RDONLY（只读）、O_WRONLY（只写）、O_RDWR（读写） 常用附加标志： O_CREAT 文件不存在则创建 O_EXCL 与 O_CREAT 同用，若文件已存在则失败 O_TRUNC 打开时清空文件 O_APPEND 写入时追加到末尾 O_NONBLOCK 非阻塞模式
- `mode`：新建文件权限（仅在 O_CREAT 时有效），如 0644

**返回值与错误**

- 成功：返回非负文件描述符
- 失败：返回 *-1*，并设置 *errno*（如 *EACCES* 权限不足、*ENOENT* 文件不存在等）

## 注意事项

**注意事项**

- 打开后必须调用 *close(fd)* 释放资源，否则可能导致**文件描述符泄漏**。
- 新建文件的实际权限会受 **umask** 影响，例如 *mode=0666* 且 umask=0022，则最终权限为 *0644*。
- 对于需要高性能或底层控制的场景，推荐使用 `open()` 搭配系统调用；普通文本处理可用更高层的 `fopen()`。

## flags参数的取值

`open()`函数的`flags`参数是一个**位掩码（bitmask）**，通过按位或（`|`）组合多个标志来控制文件打开的方方面面。这些标志主要可以分为几大类。

| 标志位                      | 用途                                          | 主要来源 |
| :-------------------------- | :-------------------------------------------- | :------- |
| **访问模式 (必须指定一个)** |                                               |          |
| `O_RDONLY`                  | 只读打开                                      |          |
| `O_WRONLY`                  | 只写打开                                      |          |
| `O_RDWR`                    | 读写打开                                      |          |
| **文件创建与存在性控制**    |                                               |          |
| `O_CREAT`                   | 文件不存在则创建                              |          |
| `O_EXCL`                    | 与`O_CREAT`同用，文件已存在则报错             |          |
| `O_TRUNC`                   | 以写方式打开时，将文件截断为0长度             |          |
| `O_APPEND`                  | 追加模式，每次写操作前将文件偏移量设到末尾    |          |
| **I/O 行为控制**            |                                               |          |
| `O_SYNC`                    | 同步写，直到数据被物理写入硬件才返回          |          |
| `O_DSYNC`                   | 同步写，但仅要求数据完整性（不要求状态）      |          |
| `O_ASYNC`                   | 当I/O可能时产生信号                           |          |
| `O_NONBLOCK` / `O_NDELAY`   | 非阻塞模式打开，`open()`本身和后续I/O不会阻塞 |          |
| **目录与链接控制**          |                                               |          |
| `O_DIRECTORY`               | 要求`pathname`必须是目录，否则失败            |          |
| `O_NOFOLLOW`                | 如果`pathname`是符号链接，则`open()`失败      |          |
| **特殊用途**                |                                               |          |
| `O_NOCTTY`                  | 如果`pathname`是终端设备，防止其成为控制终端  |          |
| `O_CLOEXEC`                 | 在执行`exec`时自动关闭此文件描述符            |          |
| `O_DIRECT`                  | 文件I/O绕过内核缓冲区高速缓存                 |          |
| `O_LARGEFILE`               | 在32位系统上支持打开大文件                    |          |
| `O_NOATIME`                 | 读取文件时不更新最后访问时间（需特定权限）    |          |

### 	标志详解

- **访问模式**：这是**必须指定且只能指定一个**的基础标志，决定了文件最基本的操作权限。
  - `O_RDONLY`：只读。
  - `O_WRONLY`：只写。
  - `O_RDWR`：读写。
- **文件创建与控制**：这些标志通常与访问模式组合使用。
  - **`O_CREAT`**：这是创建新文件的关键。当它被使用时，`open()`函数需要提供**第三个参数`mode`**，用于指定新文件的权限（例如`0644`）。这个权限值最终会受到进程`umask`的影响。
  - **`O_CREAT | O_EXCL`**：这是一个非常有用的组合，用于**原子性地检查并创建文件**。如果文件已存在，`open()`会调用失败并返回`EEXIST`错误。这在需要防止多个进程同时创建同一个文件（例如实现文件锁或守护进程单例）时至关重要。
  - **`O_TRUNC`**：常与`O_WRONLY`或`O_RDWR`配合使用，在打开文件的同时**清空其内容**，相当于先删后写。
  - **`O_APPEND`**：确保每次写入的数据都**追加到文件末尾**，是写日志文件时的标准配置。这在多进程同时写同一个文件时尤其有用，可以防止数据互相覆盖。
- **I/O 行为控制**：这些标志影响`read()`和`write()`的行为。
  - **`O_NONBLOCK`**：以非阻塞方式打开文件。对于管道（FIFO）、设备文件等，如果当前没有数据可读或无法立即写入，相关I/O调用会立即返回一个错误（如`EAGAIN`），而不是让程序一直等下去。
  - **`O_SYNC`**：以同步I/O方式打开文件。每次`write()`调用都要等到数据真正写入磁盘后才返回，**极大地保证了数据安全，但性能会显著下降**。适合对数据完整性要求极高的场景（如数据库日志）。
- **目录与链接安全**：这些标志主要用于增强文件操作的安全性。
  - **`O_DIRECTORY`**：可以用来防止误操作，比如你期望打开一个目录，但传给它的是一个普通文件，这时`open()`会直接失败。
  - **`O_NOFOLLOW`**：这是一个安全标志。如果传入的路径是一个符号链接，`open()`会失败。在处理关键文件时，可以防止程序意外地跟随链接去操作一个非预期的文件

### 	如何使用

- **通过按位或（`|`）操作符来组合这些标志。**

# C++ `read()` 和 `write()` 函数原型详解

![屏幕截图 2025-10-12 205239](D:\Typora\typora_work\4多线程与并发\屏幕截图 2025-10-12 205239.png)

这两个对于文件的读写操作同样适用于socket。

但是socket编程接口提供了几个专门用于socket数据读写的系统调用，它们增加了对数据读写的控制。

- 用于TCP流数据读写的系统调用是：`recv`/`send`

- 用于UDP数据报的系统调用是：`recvfrom`/`sendto`

### 1. **POSIX/UNIX 系统调用函数**

这两个函数属于低级I/O系统调用，定义在 `<unistd.h>` 头文件中：

#### **`read()` 函数原型**

```cpp
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count);
```

- `read`的数据流向是：从内核空间到用户空间，它的`buf`是“**目的地**”
- **用户空间←内核空间**，内核是数据的**提供者**，用户是数据的**消费者**。

**参数：**

- `fd`：文件描述符（file descriptor）
- `buf`：读取数据存放的缓冲区指针
- `count`：请求读取的字节数

**返回值：**

- 成功：返回实际读取的字节数（0表示到达文件末尾）
- 失败：返回-1，并设置 `errno`

#### **`write()` 函数原型**

- 第二个参数是const见：[5网络编程\2TCP协议\2TCP协议.md](D:\Typora\typora_work\5网络编程\2TCP协议\2TCP协议.md)上的：`数据的传输： 读/写`

```cpp
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);
```

**参数：**

- `fd`：文件描述符
- `buf`：要写入的数据缓冲区指针，它负责告诉所有调用者：“**给我一个文件描述符`fd`、一块我只读不写的数据`buf` 和一个这个数据的长度，我负责将它写出去** ”
  - `write`的数据流向方向是 ：从用户空间到内核空间，它的buf是“**源**”
  - **内核空间←用户空间**，用户是数据的**提供者**，内核是数据的**消费者。**

- `count`：请求写入的字节数

**返回值：**

- 成功：返回实际写入的字节数
- 失败：返回-1，并设置 `errno`

---

### 2. **基本使用示例**

```cpp
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>

int main() {
    const char* text = "Hello, World!";
    
    // 写入文件
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0644);//只写打开，文件不存在则创建
    if (fd == -1) {
        perror("open failed");
        return 1;
    }
    
    ssize_t written = write(fd, text, strlen(text));
    if (written == -1) {
        perror("write failed");
    } else {
        std::cout << "Wrote " << written << " bytes\n";
    }
    close(fd);
    
    // 读取文件
    fd = open("test.txt", O_RDONLY);//只读打开
    if (fd == -1) {
        perror("open failed");
        return 1;
    }
    
    char buffer[1024];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read failed");
    } else {
        buffer[bytes_read] = '\0';  // 添加字符串结束符
        std::cout << "Read " << bytes_read << " bytes: " << buffer << "\n";
    }
    close(fd);
    
    return 0;
}
```

---

### 3. **完整套接字通信示例**

```cpp
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>

void socket_example() {
    // 创建套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 绑定地址
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);
    
    // 接受连接
    int client_fd = accept(server_fd, nullptr, nullptr);
    
    // 读取客户端数据
    char buffer[1024];
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        std::cout << "Received: " << buffer << "\n";
        
        // 发送响应
        const char* response = "HTTP/1.1 200 OK\r\n\r\nHello from server";
        write(client_fd, response, strlen(response));
    }
    
    close(client_fd);
    close(server_fd);
}
```

---

### 4. **重要特性和注意事项**

#### **阻塞与非阻塞模式**

```cpp
// 设置非阻塞模式
#include <fcntl.h>

int set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}
```

#### **部分读取/写入处理**

```cpp
// 完整读取指定字节数
ssize_t read_full(int fd, void* buf, size_t count) {
    size_t total = 0;
    char* ptr = static_cast<char*>(buf);
    
    while (total < count) {
        ssize_t n = read(fd, ptr + total, count - total);
        if (n <= 0) {
            if (n == 0) return total;  // EOF
            if (errno == EINTR) continue;  // 被信号中断
            return -1;  // 错误
        }
        total += n;
    }
    return total;
}

// 完整写入指定字节数
ssize_t write_full(int fd, const void* buf, size_t count) {
    size_t total = 0;
    const char* ptr = static_cast<const char*>(buf);
    
    while (total < count) {
        ssize_t n = write(fd, ptr + total, count - total);
        if (n <= 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        total += n;
    }
    return total;
}
```

---

### 5. **错误处理示例**

```cpp
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>

ssize_t safe_read(int fd, void* buf, size_t count) {
    ssize_t result;
    
    do {
        result = read(fd, buf, count);
    } while (result == -1 && errno == EINTR);  // 被信号中断时重试
    
    if (result == -1) {
        // 检查具体错误类型
        switch (errno) {
            case EAGAIN:  // 或 EWOULDBLOCK
                std::cerr << "Resource temporarily unavailable\n";
                break;
            case EBADF:
                std::cerr << "Bad file descriptor\n";
                break;
            case EFAULT:
                std::cerr << "Bad buffer address\n";
                break;
            case EINVAL:
                std::cerr << "Invalid arguments\n";
                break;
            case EIO:
                std::cerr << "I/O error\n";
                break;
            default:
                std::cerr << "read error: " << strerror(errno) << "\n";
        }
    }
    
    return result;
}
```

---

### 6. **与C++流对比**

| 特性           | `read()/write()`      | C++ `iostream`       |
| -------------- | --------------------- | -------------------- |
| **性能**       | 较低开销，无缓冲      | 有缓冲，可能更高性能 |
| **二进制数据** | 直接处理              | 需要特殊处理         |
| **错误处理**   | 通过 `errno` 和返回值 | 异常或流状态         |
| **控制粒度**   | 字节级精确控制        | 更高级抽象           |
| **跨平台**     | 主要在UNIX系统        | 标准C++，可移植      |

---

### 7. **常见应用场景**

1. **网络编程**（套接字通信）
2. **管道/命名管道通信**
3. **设备文件操作**
4. **内存映射文件**
5. **低级文件操作**
6. **实现自定义I/O缓冲**

### 8. **性能优化技巧**

```cpp
// 使用更大的缓冲区
char buffer[8192];  // 8KB缓冲区
ssize_t n;
while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
    // 处理数据
}

// 分散/聚集I/O（适用于多个缓冲区）
#include <sys/uio.h>

struct iovec iov[2];
char buf1[256], buf2[512];
iov[0].iov_base = buf1;
iov[0].iov_len = sizeof(buf1);
iov[1].iov_base = buf2;
iov[1].iov_len = sizeof(buf2);

ssize_t nread = readv(fd, iov, 2);
```

这些是底层的系统调用函数，在需要高性能、细粒度控制的场景下非常有用，但在日常C++编程中，通常更推荐使用C++标准库的流和容器进行I/O操作。

# ==----------------------==

# Socket编程——创建`socket`

英文含义：**插座**

`socket` 函数用于创建一个新的套接字，它是网络编程中最基础的函数之一。其原型如下：

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

### 参数说明

1. **`domain`**（协议域/地址族）
   指定套接字使用的通信协议族，常见的取值有：
   - **AF_INET**：`IPv4` 协议
   - **AF_INET6**：`IPv6` 协议
   - **AF_UNIX** 或 **AF_LOCAL**：`Unix` 域协议（用于同一台机器上的进程间通信）
   - **AF_PACKET**：底层包接口（`Linux` 特有）
2. **`type`**（套接字类型）
   指定套接字的通信语义，常用的取值有：
   - **SOCK_STREAM**：提供有序、可靠、双向、基于连接的字节流（对应 TCP）
   - **SOCK_DGRAM**：提供不可靠、无连接的数据报（对应 UDP）
   - **SOCK_RAW**：原始套接字，允许直接访问底层协议（需要特权）
   - **SOCK_SEQPACKET**：有序、可靠、基于连接的数据报（保留消息边界）
3. **`protocol`**（协议）
   指定实际使用的传输协议。通常设置为 `0`，表示根据 `domain` 和 `type` 的组合选择默认协议。例如：
   - 对于 `AF_INET` + `SOCK_STREAM`，默认协议是 `IPPROTO_TCP`（TCP）
   - 对于 `AF_INET` + `SOCK_DGRAM`，默认协议是 `IPPROTO_UDP`（UDP）
     如果需要显式指定，可以使用如 `IPPROTO_TCP`、`IPPROTO_UDP`、`IPPROTO_SCTP` 等常量。

### 返回值

- **成功**：返回一个非负整数的**文件描述符**，代表新创建的套接字。
- **失败**：返回 `-1`，并设置 `errno` 以指示错误原因（如 `EPROTONOSUPPORT`、`EMFILE` 等）。

# Socket套接字

socket是一个编程接口（网络编程接口）。是一种特殊的**文件描述符**（ `write/read` ）。`socket`并不仅限于`TCP/IP`。

`socket`独立于具体协议的编程接口，这个接口位于**`TCP/IP`四层模型的应用层和传输层**之间。

![v2-99a05a19739d54b13fb53a8506d3985e_1440w](D:\Typora\typora_work\5网络编程\2TCP协议\v2-99a05a19739d54b13fb53a8506d3985e_1440w.jpg)

## 宏定义

- `INADDR_ANY` 是一个特殊的IPv4地址常量，通常用于服务器编程中，表示监听所有可用的网络接口。

```c++
#define INADDR_ANY ((in_addr_t) 0x00000000)  // 用16进制表示的32位整数(16进制一位数字表示4bit)
//点分十进制：0.0.0.0 --用点分割十进制，一个十进制数表示8bit，4个十进制数，一共8*4 = 32bit
//十六进制：00 00 00 00) ---一个16进制数表示4bit，8个十六进制数，一共4*8 = 32bit

#include <netinet/in.h>
#define INET_ADDRSTRLEN 16;//ipv4
#define INET6_ADDRSTRLEN 46;//ipv6
```

## 协议族、地址族和域

- **地址族（Address Family，AF）** --==全部用地址族表示准没错==
  - **定义**：地址的格式和表示方式
  - **示例**：`AF_INET`（IPv4地址）、`AF_INET6`（IPv6地址）、`AF_UNIX`（文件路径地址）
  - **作用**：指定地址的**数据结构格式**
- **协议族（Protocol Family，PF）**
  - **定义**：通信协议的整体套件
  - **示例**：`PF_INET`（IPv4协议套件）、`PF_INET6`（IPv6协议套件）、`PF_UNIX`（本地通信）
  - **作用**：指定通信的**协议栈**
- **域（Domain）**
  - **定义**：通信的"领域"或"范围"
  - **在socket()函数中**：第一个参数叫`domain`
  - **常用值**：`AF_INET`、`AF_INET6`、`AF_UNIX`等
  - **在Linux中**：`domain`参数实际上最终接收的是**协议族（`PF_*`）**

### 历史演变

```c++
// 早期BSD socket设计时：
// PF_xxx 用于socket()创建
// AF_xxx 用于地址结构

// 但现在大多数系统已让它们等价
#define AF_INET  PF_INET
#define AF_INET6 PF_INET6
#define AF_UNIX  PF_UNIX
```

| 域（socket()参数）        | 协议族（系统常量）         | 地址族（地址结构） | 地址格式                        |
| :------------------------ | :------------------------- | :----------------- | :------------------------------ |
| `AF_UNIX` / `PF_UNIX`     | `UNIX`本地通信             | `AF_UNIX`          | 文件系统路径名                  |
| `AF_INET` / `PF_INET`     | `PF_INET` IPv4协议套件     | `AF_INET`          | 32位IPv4地址+16位端口           |
| `AF_INET6` / `PF_INET6`   | `PF_INET6` IPv6协议套件    | `AF_INET6`         | 128位IPv6地址+16位端口+流信息等 |
| `AF_PACKET` / `PF_PACKET` | `PF_PACKET` 原始数据包访问 | `AF_PACKET`        | 网络接口层地址                  |

## 主机字节序和网络字节序转换：

- 现代PC大多数采用**小端字节序**，因此小端字节序又被称为**主机字节序**
- 为了解决两台使用不同字节序的主机之间直接传递导致接收端错误解释，采用一下方法：
  - 发送端总是将要发送的数据转换为 大端字节序
  - 接受端总是接受 这个大端字节序，在根据自己采用的字节序决定是否再进行转换
- **大端字节序**又称为 **网络字节序**
- 即使是同一台机子的两个进程也要考虑字节序问题，如`java虚拟机采用大端字节序`
- 输入：32位整数，输出：32位整数

```c++
#include <netinet/in.h>
//host->net long net short
unsigned long int htonl(usingned long int hostlong);//主机字节序-》网络字节序
unsigned short int htons(usingned short int hostshort);//主机字节序-》网络字节序
//net->host long host short
unsigned long int ntohl(usingned long int netlong);//网络字节序-》主机字节序
unsigned short int ntohs(usingned short int netshort)//网络字节序-》主机字节序
```

- 长整形`long int`函数常用来转换IP地址
- 短整形`long short`函数常用来转换端口号

## 通用socket地址

```c++
struct sockaddr//16字节
{
	sa_family_t sin_family; // 指定地址族，2字节
	char sa_data[14];//14字节
};
```

## 新的通用socket地址

```c++
struct sockaddr_storage
{
	sa_family_t sin_family;
	unsigned long int __ss_align;
	char __ss_padding(128-sizeof(__sss_align));
}
```

## 专用socket地址

```c++
struct sockadd_un
{
    sa_family_t sin_family;//指定地址族  2字节
    char sun_path[108];//文件路径名
}

struct sockaddr_in// 16字节 (IPv4套接字地址)
{
	sa_family_t sin_family; // 指定地址族 2字节
	u_int16_t sin_port; // 端口号 2字节
	struct in_addr sin_addr;// IP地址 4字节
	char sin_zero[8]; // 填充8字节，为了和其他协议簇地址结构体大小一样
};
struct in_addr
{
    u_int32_t s_addr;//32位ipv4地址，用网络字节序表示
}

struct sockaddr_in6 {    // 28字节 (IPv6套接字地址)
    sa_family_t     sin6_family;   // 2字节
    in_port_t       sin6_port;     // 2字节
    uint32_t        sin6_flowinfo; // 4字节
    struct in6_addr sin6_addr;     // 16字节
    uint32_t        sin6_scope_id; // 4字节

};
struct in6_addr
{
    ussigned char sa_addr[16];//ipv4地址，用网络字节序表示
}

struct sockaddr_in sock_info;
sock_info.sin_family = AF_INET; // 指定为IPV4
sock_info.sin_port = htons(6666); //指定为6666端口
sock_info.sin_addr.s_addr = inet_addr("192.168.31.1"); // 绑定ip地址 法1
// //in_addr_t的函数原型:in_addr_t inet_addr(const char* strptr);将字符串strptr转换后的结果用 返回值接收

// inet_aton("192.168.31.1",&sock_info.sin_addr);//绑定ip地址 法2
// //inet_aton的函数原型：int inet_aton(const char* cp,struct in_addr* inp);将字符串cp 转换后的结果储存到参数inp指向的结构体中（这个结构体存储的就是ipv4地址的网络字节序）
//inet_pton(AF_INET,"192.168.31.1"，&sock_info.sin_addr)
```

![v2-48f56688737085cf83d132b6f0462ad3_r](D:\Typora\typora_work\5网络编程\2TCP协议\v2-48f56688737085cf83d132b6f0462ad3_r.jpg)

## IP地址转换函数

==73页==

```c++
#include <arpa/inet.h>
in_addr_t inet_addr(const char* strptr);//IPv4地址字符串 -》网络字节序
int inet_aton(const char* cp,struct in_addr* inp);//网络字节序 -》Iv4P地址字符串
//struct in_addr
//{
//    u_int32_t s_addr;//32位ipv4地址，用网络字节序表示
//}

char* inet_ntoa(struct in_addr in);
```

- `inet_addr`函数将用点分十进制字符串表示的IPV4地址转换为用 网络字节序整数表示的IPV4地址(**用返回值来接收**)，失败时返回`INADDR NONE` 
  - 输入：字符串
  - 输出：32位整数(网络字节序) 

- `inet_aton`函数完成和`inet_addr`相同的功能，但是它将转换后的结果储存到参数inp指向的结构体中(**用指向结构体的指针来接收**，这个结构体存储的就是ipv4地址的网络字节序)，成功返回1，失败返回0
  - 输入：字符串
  - 输出：结构体，指向该结构体的指针内部存放转换后的 32位整数(网络字节序)
- `inet_ntoa`函数将用网络字节序整数表示的IPV4地址转换为用点分十进制字符串表示的IPV4地址。指的注意的是：它的函数内部用一个静态变量存储转换结果，该函数的 返回值指向这个 静态内存，因此`inet_ntoa`是不可重入的，即它的结果始终是一个值。
  - 输入:字符串
  - 输出：指向存储  32位整数(网络字节序) 的 静态变量



- #### 以下这两对新函数也能完成前面3个函数同样的功能，并且同时适用IPV4和IPV6地址

```c++
#include <arpa/inet.h>
int inet_pton(int af,const char* src,void* dst);//IP地址字符串 -》网络字节序
const char* inet_ntop(int af,const void* src,char* dst,socklen_t cnt);//网络字节序 -》IP地址字符串
```

- ## 参数

  - 其中`src`参数表示**用字符串表示的IP地址**

  - `af`参数表示**指定地址族**

  - `dst`参数表示转换后的结果存储与`dst`指向的内存

  - `cnt`参数用来指定目标存储单元的大小

    ```c++
    #include <netinet/in.h>
    #define INET_ADDRSTRLEN 16;//ipv4
    #define INET6_ADDRSTRLEN 46;//ipv6
    ```

  - `inet_pton` 成功返回1，失败返回0，并设置`errno`

  - `inet_ntop`成功返回目标存储单元的地址，失败返回NULL并设置`errno`

# socket类型

## 	基本类型（必须指定）

- **SOCK_STREAM**：面向连接的流套接字（TCP）针对于传输层协议为TCP的应用
- **SOCK_DGRAM**：无连接的数据报套接字（UDP）针对于传输层协议为`UDP`的应用
- **SOCK_RAW**：原始套接字 直接跳过传输层
- **SOCK_SEQPACKET**：有序分组套接字

##  	可选标志（可按位或添加）

### **SOCK_NONBLOCK**：设置非阻塞模式

```c++
// 传统方式：先创建，再用fcntl设置非阻塞
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
fcntl(sockfd, F_SETFL, O_NONBLOCK);

// 新方式：创建时直接指定（原子操作）
int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);//非阻塞套接字,非阻塞I/O
```

- 特点：
  - **立即返回**：读写操作不会阻塞进程
  - **EAGAIN/EWOULDBLOCK**：当没有数据可读或缓冲区满时返回这些错误
  - **异步I/O**：配合select/poll/epoll使用，实现高性能I/O
  - **原子性**：避免创建和设置之间的竞争条件
- 示例：

```c++
#include <sys/socket.h>
#include <fcntl.h>

// 创建非阻塞TCP套接字
int create_nonblocking_tcp_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }
    return sockfd;
}

// 创建非阻塞UDP套接字
int create_nonblocking_udp_socket() {
    int sockfd = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }
    return sockfd;
}
```

- 使用场景：
  - **高性能服务器**：epoll + 非阻塞套接字
  - **实时系统**：需要避免I/O阻塞
  - **异步编程**：配合事件循环使用
  - **防止死锁**：避免在单一连接上阻塞整个进程

- 注意事项：
  - 需要配合错误处理使用
  - 使用`accept4()`也可以直接创建非阻塞套接字
  - 非阻塞读写需要检查errno是否为`EAGAIN`或`EWOULDBLOCK`
  - 适用于需要处理大量并发连接的场景

### **SOCK_CLOEXEC**：设置close-on-exec标志

**CLOEXEC（Close-on-Exec）**含义见下文

# ==--------------------------==

# 文件描述符标志和文件状态标志

文件描述符 fd 

标志flags

状态 status

| 维度         | 文件描述符标志（fd flags）   | 文件状态标志（file status flags）       |
| :----------- | :--------------------------- | :-------------------------------------- |
| **作用对象** | fd本身（描述符级别）         | 打开的文件表项（open file description） |
| **共享性**   | 进程私有，不共享             | 在dup/fork后共享                        |
| **继承性**   | 不被dup继承                  | 被dup继承                               |
| **操作命令** | `fcntl(fd, F_GETFD/F_SETFD)` | `fcntl(fd, F_GETFL/F_SETFL)`            |

```c
进程A
├── 文件描述符 0 (stdin)  ----> 内核打开文件表条目A [文件状态标志: O_RDONLY]
│                                   ⬇
├── 文件描述符 1 (stdout) ----> 内核打开文件表条目B [文件状态标志: O_WRONLY | O_APPEND]
│       (FD_CLOEXEC=1)                ⬇
└── 文件描述符 4        -----------↗      磁盘上的文件B (inode)
    (FD_CLOEXEC=0)                           [文件状态： 权限、大小、时间...]
```

### 1. **文件描述符标志（File Descriptor Flags）**

这些标志**附着在文件描述符本身**，而不是打开的文件上。

#### 文件描述符的唯一标志位:(详见下文)

- 文件描述符的**唯一标志位**：**FD_CLOEXEC**  

  - 通过 `fcntl()` 系统调用**设置已打开文件描述符表**的 `close-on-exec` 标志

    - 使用**fcntl**改变**文件描述符标志**只能是  **FD**  `F_GETFD / F_SETFD`:

    ```c
    // 操作命令：F_GETFD / F_SETFD
    
    // 获取文件描述符标志
    int flags = fcntl(fd, F_GETFD);
    // 设置文件描述符标志 ---打开后设置
    fcntl(fd, F_SETFD, flags | FD_CLOEXEC);// 设置close-on-exec
    ```

  - 在 `open()`、`openat()` 等文件打开函数中指定 **O_CLOEXEC** ，在**打开文件时设置** `close-on-exec` 标志，

    - **打开文件时**设置**文件描述符标志位**：

    ```c
    //文件
    int fd = open("file", O_RDWR | O_CLOEXEC);//读写打开，在执行exec时自动关闭此文件描述符
    // 管道
    int pipefd[2];
    pipe2(pipefd, O_CLOEXEC);
    ```

  - 在 `socket()`、`accept4()`、`socketpair()` 等socket相关函数中指定**SOCK_CLOEXEC**，在**创建socket时设置** `close-on-exec` 标志

    - **创建socket时**设置**文件描述符标志位**：

    ```c
    //套接字
    int sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
    ```


### 2. **文件状态标志（File Status Flags）**

这些标志**附着在打开的文件表项**上，影响I/O操作的行为。

````c
// 操作命令：F_GETFL / F_SETFL
// 获取文件状态标志 
int flags = fcntl(fd, F_GETFL);
// 添加非阻塞标志
fcntl(fd, F_SETFL, flags | O_NONBLOCK);//O_NONBLOCK：非阻塞I/O
// 添加同步写入标志
fcntl(fd, F_SETFL, flags | O_SYNC);//O_SYNC / O_DSYNC：同步写入
````

#### 主要标志：

- **主要标志**:

  - **O_RDONLY / O_WRONLY / O_RDWR**：访问模式

  - **O_NONBLOCK**：非阻塞I/O

  - **O_APPEND**：追加模式

  - **O_ASYNC**：异步I/O通知

  - **O_DIRECT**：直接I/O

  - **O_SYNC / O_DSYNC**：同步写入


- 使用**fcntl**改变**文件状态标志**只能是：**FL**  `F_GETFL / F_SETFL`

```c
// 获取文件状态标志
int flags = fcntl(fd, F_GETFL);
// 添加非阻塞标志
fcntl(fd, F_SETFL, flags | O_NONBLOCK);//O_NONBLOCK：非阻塞I/O
// 添加同步写入标志
fcntl(fd, F_SETFL, flags | O_SYNC);//O_SYNC / O_DSYNC：同步写入
```

- **创建socket时**设置**文件状态标志**

```c
int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);//非阻塞套接字,非阻塞I/O
```

# ==--------------------==

# fcntl系统调用

- 见 `Linux高性能服务器编程/6高级IO函数.md`

# ==----------------------==

# **CLOEXEC（Close-on-Exec）**---==文件描述符标志==

**CLOEXEC** 是 **Close on Execute** 的缩写，用于控制文件描述符在执行 `exec()` 系列函数时的行为。

- `exec()` 是一系列函数的统称，这些函数用于在当前进程中执行一个新的程序。当调用 `exec()` 函数时，它会将当前进程的代码、数据、堆栈等替换为新程序的代码、数据、堆栈，然后从新程序的入口点开始执行。这意味着调用 `exec()` 后，原进程的程序不再运行，而是被新程序替代。
- exec()函数见后文

当一个进程使用 `fork()` 创建子进程后，子进程通常会继承父进程所有打开的文件描述符。如果子进程随后调用 `exec()` 来执行一个新程序，默认情况下这些文件描述符仍然保持打开状态。

## 默认情况（无 CLOEXEC）：

```c
// 父进程
int fd = open("file.txt", O_RDONLY);//只读打开
fork();  // 子进程继承 fd
exec();  // 子进程执行新程序，仍然可以访问 fd
```

## 设置了 CLOEXEC：

```c
// 父进程
int fd = open("file.txt", O_RDONLY | O_CLOEXEC);
// 或者：fcntl(fd, F_SETFD, FD_CLOEXEC);
fork();  // 子进程继承 fd
exec();  // 子进程执行新程序时，fd 被自动关闭
```

## 主要用途

- **安全性**：防止敏感文件描述符泄漏给新程序

  ```c
  // 打开密码文件，不希望子程序访问
  int fd = open("/etc/shadow", O_RDONLY | O_CLOEXEC);
  ```

- **资源管理**：避免不必要的文件保持打开状态

  ```c
  // 临时文件只供当前进程使用
  int tmp_fd = mkostemp(template, O_CLOEXEC);
  ```

- **避免竞争条件**：在多线程程序中，使用 `O_CLOEXEC` 可以原子性地设置标志

## 设置方式

- 最终都是设置的是文件描述符的唯一 标志位：**FD_CLOEXEC** 位。

![文件描述符标志位](D:\Typora\typora_work\5网络编程\1.1socket套接字\文件描述符标志位.png)

### 1. 打开时设置	 `O_CLOEXEC`/`SOCK_CLOEXEC`

- ## **O_CLOEXEC**

  - **使用场景**：在 `open()`、`openat()` 等文件打开函数中指定
  - **作用**：**原子性**地在**打开文件时设置** `close-on-exec` 标志，避免竞态条件
  - **示例**：

```c
// 文件
int fd = open("file", O_RDWR | O_CLOEXEC);

// 管道
int pipefd[2];
pipe2(pipefd, O_CLOEXEC);
```

- ## **SOCK_CLOEXEC**

  - **使用场景**：在 `socket()`、`accept4()`、`socketpair()` 等socket相关函数中指定

  - **作用**：**原子性**地在**创建socket时设置** `close-on-exec` 标志

  - **示例**：

```c
// 套接字
int sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
```

### 2. 打开后设置	`FD_CLOEXEC`

- ## **FD_CLOEXEC**

  - **使用场景**：通过 `fcntl()` 系统调用**设置已打开文件描述符表**的 `close-on-exec` 标志
  - **作用**：为已经存在的文件描述符设置“执行时关闭”属性
  - **示例**：

```c
#include <fcntl.h>
int flags = fcntl(fd, F_GETFD);// 获取文件描述符标志
fcntl(fd, F_SETFD, flags | FD_CLOEXEC);// 设置文件描述符标志 close-on-exec
```

## 关键区别

| 特性         | FD_CLOEXEC           | O_CLOEXEC      | SOCK_CLOEXEC           |
| :----------- | :------------------- | :------------- | :--------------------- |
| **使用方式** | fcntl()的F_SETFD参数 | open()的标志位 | socket/accept4的标志位 |
| **原子性**   | ❌ 非原子操作         | ✅ 原子操作     | ✅ 原子操作             |
| **竞态条件** | 存在竞态             | 无竞态         | 无竞态                 |
| **应用对象** | 已存在的fd           | 新打开的文件   | 新创建的socket         |

- 传统使用 `FD_CLOEXEC` 的方式(打开后设置)存在**竞态条件**：

  ```c
  // 传统方式（有竞态）：
  int fd = open("file", O_RDONLY);
  // 在这两步之间可能发生fork+exec
  fcntl(fd, F_SETFD, FD_CLOEXEC); 
  
  // 现代方式（无竞态）：
  int fd = open("file", O_RDONLY | O_CLOEXEC);  // 一步完成
  
  // 传统方式创建套接字并设置close-on-exec
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  // ↓ 在这两个步骤之间，可能发生fork()+exec()，子进程会继承这个socket！
  fcntl(sock, F_SETFD, FD_CLOEXEC)
  // 现代方式（无竞态）：
  int sock = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  ```

## 重要性

1. **优先使用原子操作**：尽量使用 `O_CLOEXEC` 和 `SOCK_CLOEXEC` 避免发生竞争。
2. **减少资源泄漏**：确保子进程不会无意中保持文件打开
3. **安全最佳实践**：对于敏感文件描述符，应该总是设置 CLOEXEC

**CLOEXEC** 是一种重要的文件描述符管理机制，它确保在执行 `exec()` 时自动关闭不需要继承的文件描述符，从而提高程序的安全性和资源管理效率。在现代编程中，推荐尽可能使用带有 `CLOEXEC` 标志的系统调用。

# ==------------------==

# exec()函数

**`exec()`** 是 Unix/Linux 系统中的一个**系统调用族**，用于**在当前进程的内存空间中执行一个新的程序**。它**替换**了当前进程的代码、数据、堆栈等，使其成为一个全新的进程。

## 核心概念

### 1. **替换而不是创建新进程**

- `fork()` 创建一个新的进程（子进程），内存内容是父进程的副本
- `exec()` **不创建新进程**，而是在**当前进程的内存空间**中加载并执行新程序
- 进程ID（PID）保持不变

```c
// 当前进程
┌─────────────┐
│ 程序A的代码  │ ← exec() 执行程序B
│ 程序A的数据  │
│ 堆、栈等     │
└─────────────┘

// exec() 之后（同一个PID）
┌─────────────┐
│ 程序B的代码  │ ← 完全替换
│ 程序B的数据  │
│ 新的堆、栈   │
└─────────────┘
```

### 2. **exec 函数族**

有多个 `exec` 函数，功能相同但参数传递方式不同：

```c
#include <unistd.h>

// 1. 指定完整路径，参数列表
int execl(const char *path, const char *arg0, ..., (char *)NULL);
// 示例：execl("/bin/ls", "ls", "-l", NULL);

// 2. 指定完整路径，参数数组
int execv(const char *path, char *const argv[]);
// 示例：char *args[] = {"ls", "-l", NULL}; execv("/bin/ls", args);

// 3. 在PATH中查找可执行文件，参数列表
int execlp(const char *file, const char *arg0, ..., (char *)NULL);
// 示例：execlp("ls", "ls", "-l", NULL); // 自动在PATH中查找ls

// 4. 在PATH中查找可执行文件，参数数组
int execvp(const char *file, char *const argv[]);

// 5. 指定环境变量
int execle(const char *path, const char *arg0, ..., (char *)NULL, char *const envp[]);
// 6. 指定环境变量（参数数组）
int execve(const char *path, char *const argv[], char *const envp[]);
```

## 工作方式

### exec() 前后的对比

| 特性           | exec() 前                  | exec() 后                           |
| -------------- | -------------------------- | ----------------------------------- |
| **进程ID**     | 保持不变                   | 保持不变                            |
| **父进程ID**   | 保持不变                   | 保持不变                            |
| **文件描述符** | 默认保持打开               | 默认保持打开（除非设置了CLOEXEC）   |
| **信号处理**   | 被捕获的信号重置为默认行为 | 被捕获的信号重置为默认行为          |
| **内存内容**   | 原程序代码和数据           | 新程序代码和数据                    |
| **命令行参数** | 原程序的参数               | 新程序的参数                        |
| **环境变量**   | 默认继承                   | 默认继承（除非用execle/execve指定） |

## 实际例子

### 示例1：基本用法

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    printf("程序开始...\n");
    
    // 执行 ls -l 命令
    execl("/bin/ls", "ls", "-l", NULL);
    
    // 如果exec成功，下面的代码永远不会执行
    // 因为当前进程已经被ls程序完全替换
    printf("这行永远不会打印！\n");
    return 0;
}
```

### 示例2：配合 fork() 使用（最常见模式）

```c
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // 子进程
        printf("子进程执行新程序...\n");
        char *args[] = {"ls", "-l", "-a", NULL};
        execvp("ls", args);
        
        // 如果exec失败才会执行到这里
        perror("exec失败");
        return 1;
    } else if (pid > 0) {
        // 父进程
        printf("父进程等待子进程...\n");
        wait(NULL);
        printf("子进程结束\n");
    } else {
        perror("fork失败");
    }
    
    return 0;
}
```

### 示例3：execve() 指定环境变量

```c
#include <unistd.h>
#include <stdio.h>

int main() {
    char *new_env[] = {
        "MY_VAR=hello",
        "PATH=/usr/bin:/bin",
        NULL
    };
    
    char *args[] = {"env", NULL};
    
    // 执行env命令，查看环境变量
    execve("/usr/bin/env", args, new_env);
    
    perror("execve失败");
    return 1;
}
```

## exec() 的参数规则

### 参数列表

- **第一个参数**：通常是程序名（argv[0]）
- **中间参数**：程序的实际参数
- **最后一个参数**：必须是 `(char *)NULL` 或 `(char *)0`

```c
// 正确
execl("/bin/ls", "ls", "-l", NULL);
execl("/bin/ls", "ls", "-l", "-a", (char *)0);

// 错误（忘记NULL）
execl("/bin/ls", "ls", "-l");  // 可能导致段错误
```

## 返回值

- **成功**：不返回（进程已被新程序替换）
- **失败**：返回 -1，并设置 `errno`
  - 常见错误：文件不存在、无执行权限、内存不足等

```c
execl("/nonexistent", "program", NULL);
if (errno == ENOENT) {
    printf("文件不存在\n");
}
```

## 常见使用场景

1. **Shell命令执行**

   ```bash
   # 当你输入 "ls -l" 时，shell内部大致这样做：
   # 1. fork() 创建子进程
   # 2. 子进程 exec("ls", "ls", "-l")
   ```

2. **进程间通信**

   ```c
   // 管道配合exec
   int pipefd[2];
   pipe(pipefd);
   
   if (fork() == 0) {
       close(pipefd[0]);  // 关闭读端
       dup2(pipefd[1], STDOUT_FILENO);  // 将标准输出重定向到管道
       execlp("ls", "ls", "-l", NULL);
   }
   ```

3. **脚本解释器**

   ```bash
   # 脚本第一行：#!/bin/bash
   # 内核会执行：exec("/bin/bash", "bash", "script.sh")
   ```

## 总结

- **`exec()` 替换当前进程**，而不是创建新进程
- **进程ID不变**，但程序代码、数据、堆栈等全部被替换
- **常与 `fork()` 配合使用**：`fork()` 创建进程，`exec()` 执行新程序
- **有多种变体**：区别在于参数传递方式（列表 vs 数组）和查找路径方式
- **失败才会返回**，成功则直接开始执行新程序

这是Unix/Linux系统中**创建新进程的标准模式**：`fork()` + `exec()`，这也是shell、守护进程等创建子进程的基本方式。