#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <cstring> // 需要包含这个头文件来使用 strcmp

int main()
{
    //读取目录test
    DIR* dir_p = opendir("test");
    if(dir_p == nullptr)
    {
        perror("打开失败");
        return -1;
    }
    
    std::cout << "打开成功" << std::endl;
    struct dirent *dt = nullptr;
    while((dt = readdir(dir_p)) != nullptr)
    {
        // 过滤掉 "." 和 ".." 这两个特殊目录
        if (strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0) {
            continue; // 跳过本次循环，不打印
        }
        std::cout << dt->d_name << std::endl;
    }
    
    closedir(dir_p); // 良好的习惯是关闭打开的目录流
    return 0;
}