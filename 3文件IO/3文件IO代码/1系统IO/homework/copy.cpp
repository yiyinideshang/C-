#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<iostream>
#include<unistd.h>

int main()
{
    int fd_a = open("A.txt",O_RDONLY);
    if(fd_a == -1)
    {
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;

    char buffer[2]={0};
    int fd_b = open("B.txt",O_WRONLY|O_CREAT,0666);

    int ret;
    while((ret = read(fd_a,buffer,sizeof(buffer)))>0)
    {
        write(fd_b,buffer,ret);
    }
    return 0;
}