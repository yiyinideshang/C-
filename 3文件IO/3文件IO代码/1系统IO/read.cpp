#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    int file_id = open("11.txt",O_RDONLY|O_CREAT,0777);
    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    // 存储读取到的内容的空间地址
    char buffer[512]={0};
    int ret = read(file_id,buffer,512);
    std::cout<<"实际读取："<<ret<<"个字节"<<std::endl;
    std::cout<<"内容："<<buffer<<std::endl;

    // open函数返回一个整数（int）类型的文件描述符（file descriptor）。
    // 如果打开文件成功，返回的值是一个非负整数，代表文件描述符。如果失败，返回-1。
    // std::cout<< file_id<<std::endl;

    close(file_id);
    return 0;
}