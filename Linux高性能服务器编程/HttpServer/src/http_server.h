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