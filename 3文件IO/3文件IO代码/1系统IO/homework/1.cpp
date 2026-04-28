#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    // fd = file_id
    int file_id = open("A.txt",O_RDONLY);
    if(file_id == -1)
    {   
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    // 存储读取到的内容的空间地址
    char buffer[3]={0};
    int ret;
    while((ret = read(file_id,buffer,2)) > 0)
    {
        buffer[ret] = '\0'; 
        std::cout<<"每次读取的字节数："<<ret<<std::endl;
        std::cout<<"每次读取的内容："<<buffer<<std::endl;
    }

    return 0;
}