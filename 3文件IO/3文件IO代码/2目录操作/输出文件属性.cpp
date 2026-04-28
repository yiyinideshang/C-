#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <string>

int main() {
    const char* dir_name = "./";
    DIR* dir_p = opendir(dir_name);
    if (dir_p == nullptr) {
        perror("打开失败");
        return -1;
    }
    std::cout << "打开成功" << std::endl;

    struct dirent *dt = nullptr;
    while ((dt = readdir(dir_p)) != nullptr) {
        // 跳过 "." 和 ".."
        if (strcmp(dt->d_name, ".") == 0 || strcmp(dt->d_name, "..") == 0) {
            continue;
        }

        // 构建完整路径
        std::string full_path = std::string(dir_name) + "/" + dt->d_name;
        struct stat st;
        if (stat(full_path.c_str(), &st) == -1) {
            perror("stat失败");
            continue;
        }

        // 判断文件类型
        std::cout << dt->d_name << ":";
        switch (st.st_mode & S_IFMT) {
            case S_IFREG: std::cout << "普通文件"; break;
            case S_IFDIR: std::cout << "目录文件"; break;
            case S_IFCHR: std::cout << "字符设备文件"; break;
            case S_IFBLK: std::cout << "块设备文件"; break;
            case S_IFIFO: std::cout << "FIFO文件"; break;
            case S_IFLNK: std::cout << "符号链接文件"; break;
            case S_IFSOCK: std::cout << "套接字文件"; break;
            default: std::cout << "未知文件类型"; break;
        }
        std::cout << ":";

        // 输出完整权限字符串（类似ls -l）
        std::cout << ( (st.st_mode & S_IRUSR) ? "r" : "-" );
        std::cout << ( (st.st_mode & S_IWUSR) ? "w" : "-" );
        std::cout << ( (st.st_mode & S_IXUSR) ? "x" : "-" );
        std::cout << ( (st.st_mode & S_IRGRP) ? "r" : "-" );
        std::cout << ( (st.st_mode & S_IWGRP) ? "w" : "-" );
        std::cout << ( (st.st_mode & S_IXGRP) ? "x" : "-" );
        std::cout << ( (st.st_mode & S_IROTH) ? "r" : "-" );
        std::cout << ( (st.st_mode & S_IWOTH) ? "w" : "-" );
        std::cout << ( (st.st_mode & S_IXOTH) ? "x" : "-" );
        std::cout << ":" << st.st_size << std::endl;
    }

    closedir(dir_p);
    return 0;
}