#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main()
{
    int file_id = open("1.txt",O_RDONLY);
    if(file_id == -1)
    {
        std::cout<<"文件打开失败"<<std::endl;
    }
    else
    {
        std::cout<<"文件打开成功"<<std::endl;
    }
}