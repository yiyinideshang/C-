#include <unistd.h>
#include <signal.h>
#include <iostream>

void alarm_handler(int sig) 
{
    std::cout << "闹钟响了！收到 SIGALRM 信号:" << sig << std::endl;
    
    // 在这里设置下一次闹钟
    alarm(3);
}

int main() 
{
    signal(SIGALRM, alarm_handler);
    
    std::cout << "设置 3 秒后响铃的闹钟" << std::endl;
    alarm(3);  // 设置第一次闹钟
    
    while(1) 
    {
        pause();  // 等待信号（信号处理函数中会重新设置闹钟）
    }
    
    return 0;
}