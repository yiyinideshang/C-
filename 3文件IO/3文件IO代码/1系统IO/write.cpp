#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    // int file_id = open("11.txt",O_RDWR|O_CREAT,0777);//这样会覆盖之前内容
    int file_id = open("11.txt",O_RDWR|O_APPEND);//这样只会追加内容

    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    // 存储读取到的内容的空间地址
    char buffer[512]={"abcdefg"};
    int ret = write(file_id,buffer,7);
    std::cout<<"实际写入："<<ret<<"个字节"<<std::endl;
    std::cout<<"写入内容："<<buffer<<std::endl;

    return 0;
}