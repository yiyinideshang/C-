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