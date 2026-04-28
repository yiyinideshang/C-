#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    //创建一个子进程：
    pid_t pid = fork();

    //fork 一次调用，两次返回。
    //父进程返回 子进程的id号（>0）
    if(pid > 0)
    {
        std::cout<<"-------------"<<std::endl;
        std::cout << "我进入的是父进程" << std::endl;
        std::cout <<"当前是父进程的ID:"<< getpid() << std::endl;
        std::cout <<"返回值的ID:"<< pid << std::endl;//父进程返回子进程的id号（>0）
        
        // 等待子进程结束
        wait(NULL);
    }
    // 子进程返回 0 
    else if(pid == 0)
    {
        std::cout<<"-------------"<<std::endl;
        std::cout << "我进入的是子进程" << std::endl;
        std::cout <<"当前是子进程的ID:"<< getpid() << std::endl;//与父进程返回结果相同
        std::cout <<"父进程的ID:"<< getppid() << std::endl;
        std::cout <<"返回值的ID:"<< pid << std::endl;
    }
    else
    {
        // fork失败的情况
        std::cerr << "fork失败" << std::endl;
        return 1;
    }
    return 0;
}