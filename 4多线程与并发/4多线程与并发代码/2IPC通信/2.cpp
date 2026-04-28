#include<iostream>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
    // FILE *file = fopen("1.txt","a+");
    pid_t pid = fork();
    if(pid>0)
    {
        std::cout<<"这是父进程"<<std::endl;
        wait(NULL);
        FILE *file = fopen("1.txt","a+");
        char buffer[1024] = {0};
        fread(buffer,1,1024,file);
        std::cout<<"通过父进程读取文件内容:"<<buffer<<std::endl;
        fclose(file);
    }
    else
    {
        FILE *file = fopen("1.txt","a+");

        std::cout<<"这是子进程"<<std::endl;
        std::string data;
        std::cout<<"请输入要输入的文件内容:";
        std::cin>>data;
        fwrite(data.c_str(),1,data.size(),file);
        fclose(file);
    }

    return 0;
}