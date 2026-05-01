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