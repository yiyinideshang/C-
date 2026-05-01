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