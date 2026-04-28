#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        // 父进程先执行
        std::cout << "父进程执行..." << std::endl;
        int status = 0;
        wait(&status);
        std::cout << "父进程结束" << std::endl;
        // 检查子进程如何退出
        if (WIFEXITED(status)) {
            std::cout << "子正常退出，退出码:" << WEXITSTATUS(status) << std::endl;
        }
        if (WIFSIGNALED(status)) {
            std::cout << "子被信号终止，信号:" << WTERMSIG(status) << std::endl;
        }
    }
    else
    {
        std::cout << "子进程执行..." << std::endl;
        for(int i = 0;i < 10;i++)
        {
            std::cout << i<< " ";
            
        }
        std::cout << std::endl;
        std::cout << "子进程结束" <<std::endl;
        exit(1);
    }

    return 0;
}