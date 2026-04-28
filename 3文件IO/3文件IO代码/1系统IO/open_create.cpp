#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main()
{
    //fd = file_id
    int file_id = open("11.txt",O_RDONLY|O_CREAT,0777);
    if(file_id == -1)
    {
        perror("文件打开失败");
        return -1;
    }
    std::cout<<"文件打开成功"<<std::endl;
    return 0;
}