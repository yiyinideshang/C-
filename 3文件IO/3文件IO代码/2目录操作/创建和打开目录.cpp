#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>
#include<dirent.h>

int main()
{
    DIR* dir_p = opendir("test");
    if(dir_p == nullptr)
    {
        perror("打开失败");
        mkdir("test",0777);
        std::cout<<"已为您创建该目录"<<std::endl;
        return 0;
    }
    std::cout<<"打开成功"<<std::endl;

    return 0;
}