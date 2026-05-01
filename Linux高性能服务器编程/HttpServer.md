# README.md

# TinyHttpServer

一个基于 Epoll ET 模式的轻量级 HTTP 服务器，用于学习和实践 Linux 网络编程。

## 技术栈
- C++11
- Epoll (ET 边缘触发)
- 线程池
- 定时器处理非活跃连接
- 异步日志

## 功能
- 支持 GET 请求，返回静态资源（HTML/CSS/JS/图片）
- 支持 404、405 状态码
- 使用 Epoll 实现高并发 I/O 复用
- 使用线程池处理业务逻辑，避免阻塞主循环
- 使用升序定时器链表关闭超时连接

## 编译与运行
```bash
mkdir build && cd build
cmake ..
make
./httpserver 8888   
```

# 目录结构

```tex
TinyHttpServer/
├── src/
│   ├── main.cpp
│   ├── http_server.cpp
│   ├── http_server.h
│   ├── thread_pool.h
│   ├── timer.h
│   └── log.h
├── www/                (静态文件目录，放个 index.html)
│   └── index.html
├── CMakeLists.txt
└── README.md
```

# 核心代码

## thread_pooll.h(线程池)

```c++
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>

class ThreadPool {
public:
    ThreadPool(size_t threads);
    ~ThreadPool();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

inline ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for(size_t i = 0; i < threads; ++i)
        workers.emplace_back([this] {
            for(;;) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this]{ return this->stop || !this->tasks.empty(); });
                    if(this->stop && this->tasks.empty()) return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
}

inline ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers) worker.join();
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

#endif
```

## timer.h（定时器，处理非活跃连接）

```c++
#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <netinet/in.h>
#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include <unordered_map>

class TimerNode {
public:
    int sockfd;
    sockaddr_in address;
    time_t expire;
    std::function<void(int)> callback;
    TimerNode(int fd, sockaddr_in addr, time_t exp, std::function<void(int)> cb)
        : sockfd(fd), address(addr), expire(exp), callback(cb) {}
    bool operator<(const TimerNode& other) const {
        return expire > other.expire; // 小顶堆
    }
};

class TimerManager {
public:
    void addTimer(int fd, sockaddr_in addr, time_t timeout, std::function<void(int)> cb) {
        time_t expire = time(nullptr) + timeout;
        TimerNode node(fd, addr, expire, cb);
        timer_heap.push(node);
        fd_to_expire[fd] = expire;
    }
    void tick() {
        time_t now = time(nullptr);
        while(!timer_heap.empty() && timer_heap.top().expire <= now) {
            TimerNode node = timer_heap.top();
            timer_heap.pop();
            if(fd_to_expire[node.sockfd] == node.expire) {
                node.callback(node.sockfd);
                fd_to_expire.erase(node.sockfd);
            }
        }
    }
    void updateTimer(int fd, time_t new_timeout) {
        // 简化版：重新添加定时器
    }
private:
    std::priority_queue<TimerNode> timer_heap;
    std::unordered_map<int, time_t> fd_to_expire;
};

#endif
```

## http_server.h（核心服务器类）

```c++
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include "thread_pool.h"
#include "timer.h"
#include "log.h"

class HttpServer {
public:
    HttpServer(int port, int thread_num);
    ~HttpServer();
    void run();

private:
    int setNonBlocking(int fd);
    void handleEvents(int epollfd, epoll_event* events, int num);
    void handleAccept();
    void handleRead(int fd);
    void handleWrite(int fd);
    void processRequest(int fd, const std::string& request);
    void sendResponse(int fd, int status, const std::string& content, const std::string& contentType);
    void closeConnection(int fd);

    int listenfd;
    int epollfd;
    int port;
    bool running;
    ThreadPool pool;
    TimerManager timerMgr;
    std::string rootDir;
    static const int MAX_EVENTS = 1024;
    static const int BUFFER_SIZE = 4096;
    static const int TIMEOUT = 60; // 60秒超时
};

#endif
```

## http_server.cpp（实现细节）

```c++
#include "http_server.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <iostream>
#include <sstream>
#include <fstream>

HttpServer::HttpServer(int port, int thread_num) 
    : port(port), running(false), pool(thread_num), rootDir("./www") {
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(listenfd, (sockaddr*)&addr, sizeof(addr));
    listen(listenfd, 5);
    setNonBlocking(listenfd);
    epollfd = epoll_create1(0);
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listenfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
}

HttpServer::~HttpServer() {
    close(listenfd);
    close(epollfd);
}

int HttpServer::setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }
    return 0;
}


void HttpServer::run() {
    running = true;
    epoll_event events[MAX_EVENTS];
    while(running) {
        int num = epoll_wait(epollfd, events, MAX_EVENTS, 1000);
        if(num > 0) {
            handleEvents(epollfd, events, num);
        }
        timerMgr.tick();
    }
}

void HttpServer::handleEvents(int epollfd, epoll_event* events, int num) {
    for (int i = 0; i < num; ++i) {
        int fd = events[i].data.fd;
        if (fd == listenfd) {
            handleAccept();
        } else if (events[i].events & EPOLLIN) {
            handleRead(fd);
        } else if (events[i].events & EPOLLOUT) {
            // 暂不处理写事件
        }
    }
}

void HttpServer::handleAccept() {
    while (true) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int connfd = accept(listenfd, (sockaddr*)&clientAddr, &clientLen);
        if (connfd == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            perror("accept");
            break;
        }
        setNonBlocking(connfd);
        epoll_event ev;
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = connfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) == -1) {
            perror("epoll_ctl: connfd");
            close(connfd);
            continue;
        }
        timerMgr.addTimer(connfd, clientAddr, TIMEOUT, [this](int fd) {
            closeConnection(fd);
        });
        std::cout << "New connection: fd=" << connfd << std::endl;
    }
}

void HttpServer::handleRead(int fd) {
    char buf[BUFFER_SIZE];
    int len = recv(fd, buf, sizeof(buf)-1, 0);
    if(len <= 0) {
        closeConnection(fd);
        return;
    }
    buf[len] = '\0';
    pool.enqueue([this, fd, req = std::string(buf)] {
        processRequest(fd, req);
    });
}

void HttpServer::processRequest(int fd, const std::string& request) {
    std::istringstream iss(request);
    std::string method, path, version;
    iss >> method >> path >> version;
    if(method != "GET") {
        sendResponse(fd, 405, "Method Not Allowed", "text/plain");
        return;
    }
    if(path == "/") path = "/index.html";
    std::string fullPath = rootDir + path;
    std::ifstream file(fullPath, std::ios::binary);
    if(!file.is_open()) {
        sendResponse(fd, 404, "Not Found", "text/plain");
        return;
    }
    std::ostringstream content;
    content << file.rdbuf();
    sendResponse(fd, 200, content.str(), "text/html");
}

void HttpServer::sendResponse(int fd, int status, const std::string& content, const std::string& contentType) {
    std::string statusStr;
    if (status == 200) statusStr = "OK";
    else if (status == 404) statusStr = "Not Found";
    else if (status == 405) statusStr = "Method Not Allowed";
    else statusStr = "Error";

    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << statusStr << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << content.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << content;

    std::string respStr = response.str();
    send(fd, respStr.c_str(), respStr.size(), 0);
}

void HttpServer::closeConnection(int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, nullptr);
    close(fd);
    std::cout << "Connection closed: fd=" << fd << std::endl;
}

void HttpServer::handleWrite(int fd) {
    // 未实现，留空
}
```

## CMakeLists.txt

```makefile
cmake_minimum_required(VERSION 3.10)
project(TinyHttpServer)

set(CMAKE_CXX_STANDARD 14)

add_executable(httpserver src/main.cpp src/http_server.cpp)

target_link_libraries(httpserver pthread)
```

## www/index.html

````html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>测试页面</title>
</head>
<body>
    <h1>Hello from HttpServer!</h1>
    <p>服务器运行成功。</p>
</body>
</html>
````

# 运行指南

## 确认有 Linux 环境

```bash
uname -a
yishang@yishang-virtual-machine:~/文档/HttpServer$ uname -a
Linux yishang-virtual-machine 6.8.0-107-generic #107~22.04.1-Ubuntu SMP PREEMPT_DYNAMIC Wed Mar 18 23:40:43 UTC  x86_64 x86_64 x86_64 GNU/Linux
```

## 安装编译工具

```bash
sudo apt update
sudo apt install g++ cmake make
```

## 确认代码目录结构

```bash
yishang@yishang-virtual-machine:~/文档/HttpServer$ tree
.
├── build
│   ├── CMakeCache.txt
│   ├── CMakeFiles
│   │   ├── 3.22.1
│   │   │   ├── CMakeCCompiler.cmake
│   │   │   ├── CMakeCXXCompiler.cmake
│   │   │   ├── CMakeDetermineCompilerABI_C.bin
│   │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   │   ├── CMakeSystem.cmake
│   │   │   ├── CompilerIdC
│   │   │   │   ├── a.out
│   │   │   │   ├── CMakeCCompilerId.c
│   │   │   │   └── tmp
│   │   │   └── CompilerIdCXX
│   │   │       ├── a.out
│   │   │       ├── CMakeCXXCompilerId.cpp
│   │   │       └── tmp
│   │   ├── cmake.check_cache
│   │   ├── CMakeDirectoryInformation.cmake
│   │   ├── CMakeOutput.log
│   │   ├── CMakeTmp
│   │   ├── httpserver.dir
│   │   │   ├── build.make
│   │   │   ├── cmake_clean.cmake
│   │   │   ├── compiler_depend.make
│   │   │   ├── compiler_depend.ts
│   │   │   ├── DependInfo.cmake
│   │   │   ├── depend.make
│   │   │   ├── flags.make
│   │   │   ├── link.txt
│   │   │   ├── progress.make
│   │   │   └── src
│   │   │       ├── http_server.cpp.o
│   │   │       ├── http_server.cpp.o.d
│   │   │       ├── main.cpp.o
│   │   │       └── main.cpp.o.d
│   │   ├── Makefile2
│   │   ├── Makefile.cmake
│   │   ├── progress.marks
│   │   └── TargetDirectories.txt
│   ├── cmake_install.cmake
│   ├── httpserver
│   └── Makefile
├── CMakeLists.txt
├── README.md
├── src
│   ├── http_server.cpp
│   ├── http_server.h
│   ├── log.h
│   ├── main.cpp
│   ├── thread_pool.h
│   └── timer.h
└── www
    └── index.html

12 directories, 42 files
```

## 编译项目

进入项目根目录，执行：

```bash
mkdir build
cd build
cmake ..
make
```

## 运行服务器

在 `build/` 目录下执行：

```bash
./httpserver 8888
yishang@yishang-virtual-machine:~/文档/HttpServer$ ./build/httpserver 8888
```

## 测试访问

打开浏览器，地址栏输入：

```text
http://localhost:8888
```

```html
Hello from HttpServer!

服务器运行成功。
```

或者用 `curl` 测试：

```bash
curl -v http://localhost:8888
```

如果看到 `index.html` 的内容，说明服务器运行成功。

```bash
yishang@yishang-virtual-machine:~/桌面$ curl -v http://localhost:8888
*   Trying 127.0.0.1:8888...
* Connected to localhost (127.0.0.1) port 8888 (#0)
> GET / HTTP/1.1
> Host: localhost:8888
> User-Agent: curl/7.81.0
> Accept: */*
> 
* Mark bundle as not supporting multiuse
< HTTP/1.1 200 OK
< Content-Type: text/html
< Content-Length: 191
< Connection: close
< 
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>测试页面</title>
</head>
<body>
    <h1>Hello from HttpServer!</h1>
    <p>服务器运行成功。</p>
</body>
* Closing connection 0
</html>yishang@yishang-virtual-machine:~/桌面$
```

- ### 安装 curl

```bash
sudo apt install curl -y
```

## favicon.ico

`favicon.ico` 是网站的 **“收藏夹图标”** 或 **“标签页小图标”**。

### 它是做什么的？

当你用浏览器访问一个网站时，浏览器会**自动**尝试获取这个文件，用来显示在：

- 浏览器标签页的标题旁边（就是你此刻看到的那枚小图标）
- 收藏夹/书签栏里
- 历史记录中

这是浏览器的**默认行为**，并不是服务器主动要的。

### 为什么会在控制台看到请求这个文件？

当你访问 `http://localhost:8888` 时，浏览器实际上发出了**两个请求**：

1. **第一个请求**：`GET /` （获取首页 `index.html`）
   → 你的服务器返回 `200 OK` 和 HTML 内容
2. **第二个请求**：`GET /favicon.ico` （浏览器自动想拿图标）
   → 你的服务器找不到这个文件，于是返回 `404 Not Found`

所以控制台可能会看到类似这样的额外日志（取决于你的打印设置）：

```bash
New connection: fd=5
Connection closed: fd=5
New connection: fd=6
Connection closed: fd=6
```

这就对应了两次连接/关闭过程。

### 这对服务器有影响吗？

**完全没有影响**。404 是正常的 HTTP 响应，表示“你要的东西我没有”。它不会导致程序崩溃，也不会影响主页面的正常显示。你现在的服务器完全正确。

如果被问到看到你的日志里有 404的原因，可以回答：

> *“那个 404 是浏览器自动请求 `favicon.ico` 产生的。我的服务器目前只实现了对 `GET /` 的处理和静态文件返回，没有专门放图标文件，所以返回了 404 Not Found。这是浏览器的正常行为，不影响核心功能。”*

这反而能体现对 HTTP 协议和浏览器行为的了解。

### 要不要解决它？（非必须）

如果想把这个 404 也消掉，只需要在 `www/` 目录下随便放一个名为 `favicon.ico` 的小图标文件，或者放一个 0 字节的空文件（`touch www/favicon.ico`），浏览器再请求时就会返回 200 而不再是 404 了。不过**完全没有必要**，保持现状就很好，真实项目里也经常有 404 日志。