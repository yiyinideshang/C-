#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include <cstring>
#include <vector>
#include <libgen.h>

// 创建目录（包括父目录）
bool createDirectory(const std::string& path) {
    struct stat st;//这个变量将用于存储文件/目录的元数据信息
    if (stat(path.c_str(), &st) == 0) {
        //stat() 函数 获取指定路径的文件系统信息
        //如果成功，返回 0 并将信息填充到 st 结构体中
        // 如果失败，返回 -1（通常是因为路径不存在或权限不足）
        // 路径已存在
        if (S_ISDIR(st.st_mode)) {
            return true; // 已经是目录
        } else {
            std::cerr << "路径已存在但不是目录: " << path << std::endl;
            return false;
        }
    }
    
    // if不成立，即结果为-1时，表明路径不存在：递归创建
    size_t pos = 0;
    std::string current;
    
    // 如果是绝对路径，从根目录开始
    if (path[0] == '/') {
        current = "/";
        pos = 1;
    }
    
    while (pos < path.length()) {
        // 在字符串 path 中从位置 pos 开始查找字符 '/' 第一次出现的位置。
        size_t next = path.find('/', pos);
        if (next == std::string::npos) {
            current = path;
        } else {
            current = path.substr(0, next);
        }
        
        if (stat(current.c_str(), &st) == -1) {
            // 目录不存在，创建它
            if (mkdir(current.c_str(), 0755) == -1) {
                std::cerr << "无法创建目录: " << current << " (错误: " << strerror(errno) << ")" << std::endl;
                return false;
            }
        } else if (!S_ISDIR(st.st_mode)) {
            std::cerr << "路径已存在但不是目录: " << current << std::endl;
            return false;
        }
        
        if (next == std::string::npos) break;
        pos = next + 1;
    }
    
    return true;
}

// 文件拷贝函数
bool fileCopy(const std::string& srcFile, const std::string& destFile) {
    std::cout << "拷贝文件: " << srcFile << " -> " << destFile << std::endl;
    
    int srcFd = open(srcFile.c_str(), O_RDONLY);
    if (srcFd == -1) {
        std::cerr << "无法打开源文件: " << srcFile << " (错误: " << strerror(errno) << ")" << std::endl;
        return false;
    }
    
    // 确保目标文件的目录存在
    char* destCopy = strdup(destFile.c_str());
    std::string destDir = dirname(destCopy);
    free(destCopy);
    
    if (!createDirectory(destDir)) {
        close(srcFd);
        return false;
    }
    
    int destFd = open(destFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) {
        std::cerr << "无法创建目标文件: " << destFile << " (错误: " << strerror(errno) << ")" << std::endl;
        close(srcFd);
        return false;
    }
    
    char buffer[4096];
    ssize_t bytesRead;
    
    while ((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0) {
        if (write(destFd, buffer, bytesRead) != bytesRead) {
            std::cerr << "写入文件失败: " << destFile << " (错误: " << strerror(errno) << ")" << std::endl;
            close(srcFd);
            close(destFd);
            return false;
        }
    }
    
    close(srcFd);
    close(destFd);
    
    if (bytesRead == -1) {
        std::cerr << "读取文件失败: " << srcFile << " (错误: " << strerror(errno) << ")" << std::endl;
        return false;
    }
    
    return true;
}

/*
    目录拷贝：
        src:源路径（要拷贝其内容的目录，如A）
        dest:目标路径（将内容拷贝到这里，如B）
*/
void dirCopy(const std::string& src, const std::string& dest) {
    // 确保目标目录存在
    if (!createDirectory(dest)) {
        std::cerr << "无法创建目标目录: " << dest << std::endl;
        return;
    }
    
    // 打开源目录
    DIR* dirp = opendir(src.c_str());
    if (dirp == nullptr) {
        std::cerr << "无法打开源目录: " << src << " (错误: " << strerror(errno) << ")" << std::endl;
        return;
    }
    
    struct dirent* entry;
    struct stat st;
    
    while ((entry = readdir(dirp)) != nullptr) {
        // 跳过 . 和 ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        std::string srcPath = src + "/" + entry->d_name;
        std::string destPath = dest + "/" + entry->d_name;
        
        // 获取文件信息
        if (stat(srcPath.c_str(), &st) == -1) {
            std::cerr << "无法获取文件信息: " << srcPath << " (错误: " << strerror(errno) << ")" << std::endl;
            continue;
        }
        
        if (S_ISDIR(st.st_mode)) {
            // 如果是目录，递归拷贝
            std::cout << "拷贝目录: " << srcPath << " -> " << destPath << std::endl;
            dirCopy(srcPath, destPath);
        } else {
            // 如果是文件，拷贝文件
            if (!fileCopy(srcPath, destPath)) {
                std::cerr << "文件拷贝失败: " << srcPath << " -> " << destPath << std::endl;
            }
        }
    }
    
    closedir(dirp);
}

// 将路径转换为绝对路径
std::string getAbsolutePath(const std::string& path) {
    if (path.empty()) return path;
    
    if (path[0] == '/') {
        return path; // 已经是绝对路径
    }
    
    char* cwd = get_current_dir_name();
    std::string absolutePath = std::string(cwd) + "/" + path;
    free(cwd);
    
    return absolutePath;
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cout << "用法: " << argv[0] << " [源目录] [目标目录]" << std::endl;
        std::cout << "示例: " << argv[0] << " /path/to/A /path/to/B" << std::endl;
        std::cout << "效果: 将A目录下的所有内容拷贝到B目录下" << std::endl;
        return -1;
    }
    
    // 获取绝对路径
    std::string src = getAbsolutePath(argv[1]);
    std::string dest = getAbsolutePath(argv[2]);
    
    std::cout << "源目录: " << src << std::endl;
    std::cout << "目标目录: " << dest << std::endl;
    
    // 检查源目录是否存在
    struct stat st;
    if (stat(src.c_str(), &st) == -1 || !S_ISDIR(st.st_mode)) {
        std::cerr << "源目录不存在或不是目录: " << src << std::endl;
        return -1;
    }
    
    std::cout << "开始拷贝..." << std::endl;
    dirCopy(src, dest);
    std::cout << "拷贝完成" << std::endl;
    
    return 0;
}